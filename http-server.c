#include "./libs/server.h"
#include "./libs/contextholder.h"
#include <stdio.h>

void defaultHandler(){
    printf("User connected to '/'\n");
}

void styleHandler(){
    printf("Loading styles...\n");
}

int main() 
{
    setMapping("/", &defaultHandler, "GET", "/index.html");    
    setMapping("/css/style.css", &styleHandler, "GET", "/css/style.css");
    run_http_server();    
}



