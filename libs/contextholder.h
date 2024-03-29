#include <netinet/in.h>
#include <sys/types.h>
#include <time.h>

#define MAX_HANDLERS 100
#define POOL_SIZE 100

struct client_info {
    int pid; // Not in esp32
    int id;
    int port;
    struct in_addr addr;
    struct tm * time;
};

struct handler {
    char * url;
    char * file;
    char * method;
    void (*fn)(void);
};

struct handler_list {
    size_t length;
    struct handler handlers[MAX_HANDLERS];
};

struct connection_pool {
    size_t length;
    struct client_info clients[POOL_SIZE];
};

struct connection_pool * getConnectionPool();
struct handler_list * getHandlersList();

void setMapping(char * url, void (*handler)(void), char * method, char * file);
void addToConnectionPool(struct client_info client);
void editFromConnectionPool(int index, int pid);
