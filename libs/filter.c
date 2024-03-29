#include "filter.h"
#include "contextholder.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

struct http_req * filterHttpRequest(char *req) 
{
    int i = 0;
    
    struct http_req * httpr = (struct http_req*) malloc(sizeof(struct http_req));

    httpr->url = (char*) malloc(255);
    httpr->method = (char*) malloc(16);

    memset(httpr->url, 0, 255);
    memset(httpr->method, 0, 16);

    while(req[i] != ' ' && req[i] != '\0') {
        httpr->method[i] = req[i];
        i++;
    }   
    httpr->method[i] = '\0';
   
    while(req[i] == ' ') i++;

    int j=0;
    while(req[i] != ' ' && req[i] != '\0') {
        httpr->url[j] = req[i];
        i++; j++;
    }
    httpr->url[j] = '\0';
    
    return httpr; 
}

int filterHandler(struct http_req * req) 
{
    
    struct handler_list * handlers = getHandlersList();
    
    // Linear search the handler
    for(int i = 0; i < handlers->length; i++)
    {
        if(strcmp(handlers->handlers[i].url, req->url) == 0)
        {
            if(strcmp(handlers->handlers[i].method, req->method) == 0)
            {
                return i;
            }
        }
    }

    // NO HANDLER FOUND
    return -1;
}

