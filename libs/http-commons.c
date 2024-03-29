#include "http-commons.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>

#define LGET 3
#define MAX_NAME 128
#define MAX_EXT 16

char * generateHttpResponse(char * fileToSend, char * fileData) 
{
    char name[MAX_NAME];
    char ext[MAX_EXT];
    char * contentType = (char*) malloc(64);

    int fi=0,j=0,k=0;

    while(fileToSend[fi] != '.')
    {
        if(j > MAX_NAME){
            return "HTTP/1.1 500 INTERNAL SERVER ERROR\r\n\r\nFile name too long!\r\n";
            break;
        }
        name[j] = fileToSend[fi];
        j++;
        fi++;
    }
    name[j] = '\0';

    fi++;

    while (fi < strlen(fileToSend)) {
        if(k > MAX_EXT) {
            return "HTTP/1.1 500 INTERNAL SERVER ERROR\r\n\r\nFile extension too long!\r\n";
            break;
        }
        ext[k] = fileToSend[fi];
        k++; 
        fi++;
    }
    ext[k] = '\0';

    if(strcmp(ext,"css") == 0) {contentType = "text/css";} 
    else if(strcmp(ext, "html") == 0) {contentType = "text/html";} 
    else if(strcmp(ext,"js") == 0) {contentType = "text/javascript";}

    char * response = (char*) malloc(strlen(contentType) + strlen(fileData) + strlen(DEF_HTTPRESPONSE) + 255);

    sprintf(response, "HTTP/1.1 200 OK\r\nContent-type: %s\r\n\r\n%s", contentType, fileData);
  
    return response;
}

char * read_page_bytes(FILE *fp)
{
    char * page = (char*) malloc(BUF_SIZE);
    size_t bytes_read = 0;
    
    while((bytes_read = fread(page, sizeof(*page), BUF_SIZE + bytes_read, fp)) > EOF) {
        if(bytes_read == BUF_SIZE) {
            page = realloc(page, bytes_read+BUF_SIZE);
        }
    };
    
    return page;
}

FILE * find_page(char *req)
{

    
    FILE * file;
    char * fl = (char*) malloc(strlen(req) + strlen(RESOURCES_DIR));

    strcpy(fl, RESOURCES_DIR);
    strcat(fl, req);

    file = fopen(fl, "r+");
    
    if(file == NULL) {
        perror("Page not found");
        return NULL;
    };

    free(fl);
    return file;
}





