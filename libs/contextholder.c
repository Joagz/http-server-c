#include "contextholder.h"
#include <stdlib.h>
#include <stdio.h>

static struct connection_pool POOL = {
    .length=0,
};

static struct handler_list HANDLERS = {
    .length=0
};

void setMapping(char *url, void (*handler)(), char * method, char * file)
{   
    HANDLERS.handlers[HANDLERS.length].url = url;
    HANDLERS.handlers[HANDLERS.length].method = method;
    HANDLERS.handlers[HANDLERS.length].fn = handler;
    HANDLERS.handlers[HANDLERS.length].file = file;

    HANDLERS.length++;
}


void addToConnectionPool(struct client_info client)
{
    POOL.clients[POOL.length] = client;
    POOL.length++;
}

struct handler_list * getHandlersList()
{
    if(HANDLERS.length == 0){
        printf("Handler list has no elements. Please define some handler for your methods using 'setMapping'.\n");
        exit(-1);
    }
    return &HANDLERS;
}

struct connection_pool * getConnectionPool()
{
    return &POOL;
}

void editFromConnectionPool(int index, int pid){
    
    POOL.clients[index].pid = pid;

}
