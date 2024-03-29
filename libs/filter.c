#include "filter.h"
#include "contextholder.h"
#include <string.h>

struct http_req filterHttpRequest(char *req) 
{
    int i = 0;
    struct http_req httpr;

    // extract request method
    do {
        httpr.method[i] = req[i];
        i++;
    } while(req[i] != ' ');
   
    // extract request url
    do {
        httpr.url[i] = req[i];
        i++;
    } while(req[i] != ' ');

    return httpr; 
}

int filterHandler(struct http_req req) 
{
    
    struct handler_list * handlers = getHandlersList();
    
    // Linear search the handler
    for(int i = 0; i < handlers->length; i++)
    {
        if(strcmp(handlers->handlers[i].url, req.url) == 0)
        {
            if(strcmp(handlers->handlers[i].method, req.method) == 0)
            {
                return i;
            }
        }
    }

    // NO HANDLER FOUND
    return -1;
}

