#include "http-commons.h"
#include "server.h"
#include "contextholder.h"
#include "filter.h"

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

void err_n_die(int exp, char * msg)
{
    if(exp == -1 || exp == EXIT_FAILURE)
    {
        perror(msg);
        printf("Exiting program...\n");
        exit(exp);
    }
}

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

    err_n_die(bind(sock, (struct sockaddr*) &server_addr, sizeof(server_addr)), 
            "Could not bind socket in server");
    
    err_n_die(listen(sock, BACKLOG), "Could not listen for connections in server"); 
    
    int current_process_count = 0;
    while(1)
    {
        int pid;
        len = sizeof(client_addr);
        memset(&client_addr, 0, len);
        int client = accept(sock, (struct sockaddr*) &client_addr, &len);
        int pid_save;
        
        if((pid = fork()) == -1) 
        {
            perror("Client connection error (can't create subprocess for client handling)");

            // Send response to client

            continue;
        } 
        else if(pid == 0) 
        {

            time_t t = time(NULL);
            
            struct tm * lt = localtime(&t);

            struct client_info c_info = {
                .pid=pid_save,
                .id=current_process_count,
                .addr=client_addr.sin_addr,
                .time=lt
            };
        
            printf("%d, %d, %d, %d %d %d Day N.%d\n",
                    c_info.pid,
                    c_info.id,
                    c_info.addr.s_addr,
                    c_info.time->tm_sec,
                    c_info.time->tm_min,
                    c_info.time->tm_hour,
                    c_info.time->tm_wday
            );

            addToConnectionPool(c_info);

            int bytes_read = 0;
            char buffer[MAX_REQUEST_SIZE];
            while((bytes_read = read(client, buffer, MAX_REQUEST_SIZE) > EOF));
            
            struct http_req req = filterHttpRequest(buffer);
            int handlerId = filterHandler(req);
            
            struct handler_list * handlerList = getHandlersList(); 
    
            void (* fn)(void) = (*handlerList).handlers[handlerId].fn;
                
            fn();

            close(client);
        }
        else if(pid > 0) {
            current_process_count++;
            pid_save = pid;
        }

    }

}





