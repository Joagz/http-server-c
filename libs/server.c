#include "http-commons.h"
#include "server.h"
#include "contextholder.h"
#include "filter.h"

#include <asm-generic/errno.h>
#include <asm-generic/socket.h>
#include <signal.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h> 
#include <netdb.h> 
#include <stdlib.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <time.h>
#include <unistd.h> 
#include <strings.h>
#include <string.h> 
#include <fcntl.h>
#include <poll.h>
#include <arpa/inet.h>

void err_n_die(int exp, char * msg)
{
    if(exp == -1 || exp == EXIT_FAILURE)
    {
        perror(msg);
        printf("Exiting program...\n");
        exit(exp);
    }
}

#define MAX_CLIENTS 10

void run_http_server() 
{
    int sock;
    socklen_t len;
    struct sockaddr_in server_addr, client_addr;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    err_n_die(sock, "Could not create a socket for the server");

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    server_addr.sin_family = AF_INET;

    int opt = 1;

    err_n_die(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)), "Could not set SO_REUSEADDR");

    err_n_die(bind(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)), 
            "Could not bind socket in server");

    err_n_die(listen(sock, BACKLOG), "Could not listen for connections in server"); 
    int current_process_count = 0;

    if(PLATFORM == 0) // esp32
    {
A:
        len        = sizeof(client_addr);
        int client = accept(sock, (struct sockaddr*) &client_addr, &len);

        memset(&client_addr, 0, len);

        char buffer[MAX_REQUEST_SIZE];
        if( read(client, buffer, MAX_REQUEST_SIZE) == EOF ) goto A;

        struct http_req * req = filterHttpRequest(buffer);
        int handlerId = filterHandler(req);

        struct handler_list * handlerList = getHandlersList(); 
        struct handler hndlr = (*handlerList).handlers[handlerId];
        void (* fn)(void) = hndlr.fn;

        fn();

        FILE * fp = find_page(hndlr.file);
        char * bytes = read_page_bytes(fp);

        char * response = generateHttpResponse(hndlr.file, bytes);

        free(req);

        write(client, response, strlen(response));

        free(response);
        shutdown(client, SHUT_RDWR);
        close(client);
    }
    else if(PLATFORM == 1) // linux
        while(1)
        {
            int pid;
            int pid_save, 
                con_client_i, 
                skip = 0;

            int client                      = accept(sock, (struct sockaddr*) &client_addr, &len);
            struct connection_pool * pool   = getConnectionPool();
            len                             = sizeof(client_addr);

            memset(&client_addr, 0, len);

            for(int i = pool->length; i > 0; i--)
            {
                if(pool->clients[i].addr.s_addr == client_addr.sin_addr.s_addr)
                    if(kill(pool->clients[i].pid, 0) == 0)
                    {
                        skip = 1;
                        con_client_i = i;
                        kill(pool->clients[i].pid, SIGTERM);
                        current_process_count--;
                        break;
                    }
            }

            if((pid = fork()) == -1) 
            {
                perror("Client connection error (can't create subprocess for client handling)");
                continue;
            } 
            else if(pid == 0) 
            {

                char buffer[MAX_REQUEST_SIZE];
                if( read(client, buffer, MAX_REQUEST_SIZE) == EOF ) continue;

                struct http_req * req = filterHttpRequest(buffer);
                int handlerId = filterHandler(req);

                struct handler_list * handlerList = getHandlersList(); 
                struct handler hndlr = (*handlerList).handlers[handlerId];
                void (* fn)(void) = hndlr.fn;

                fn();

                FILE * fp = find_page(hndlr.file);
                char * bytes = read_page_bytes(fp);

                char * response = generateHttpResponse(hndlr.file, bytes);

                free(req);

                write(client, response, strlen(response));

                free(response);
                shutdown(client, SHUT_RDWR);
                close(client);

                exit(EXIT_SUCCESS);
            }
            else if(pid > 0) {
                if(skip == 1){

                    editFromConnectionPool(con_client_i, pid);

                    continue;
                }
                time_t t = time(NULL);

                struct tm * lt = localtime(&t);

                struct client_info c_info = {
                    .pid=pid_save,
                    .id=current_process_count,
                    .addr=client_addr.sin_addr,
                    .time=lt,
                    .port=client_addr.sin_port
                };

                printf("PID: %d, ID: %d, ADDR: %d, TIME INFO: %d %d %d Day N.%d\n",
                        c_info.pid,
                        c_info.id,
                        c_info.addr.s_addr,
                        c_info.time->tm_sec,
                        c_info.time->tm_min,
                        c_info.time->tm_hour,
                        c_info.time->tm_wday
                      );

                addToConnectionPool(c_info);

                current_process_count++;
                pid_save = pid;
            }

        }

}





