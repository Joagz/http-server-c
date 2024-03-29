#include "contextholder.h"
#include <stdlib.h>
#include <stdio.h>

static struct connection_pool POOL = {
    .length=0,
    .clients={0x00}
};

static struct handler_list HANDLERS = {
    .length=0,
    .handlers={0x00}
};

void setMapping(char *url, void (*handler)(), char * method)
{   
    HANDLERS.handlers[HANDLERS.length].url = url;
    HANDLERS.handlers[HANDLERS.length].method = method;
    HANDLERS.handlers[HANDLERS.length].fn = handler;
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
