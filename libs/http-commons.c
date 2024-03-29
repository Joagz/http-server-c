#include "http-commons.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>

#define LGET 3

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

// We assume that the request is of type GET
FILE * find_page(char *req)
{
    
    int i = LGET+1, k = 0;
    FILE *file;
    char buf[MAX_INPUT] = {0};
    memset(&buf, 0, MAX_INPUT);

    while(req[i] != ' '){
        buf[k] = req[i];
        i++; k++;
    }
    
    char * fl = (char*) malloc(strlen(buf) + strlen(PAGES_FOLDER));

    strcpy(fl, PAGES_FOLDER);
    strcat(fl, buf);

    file = fopen(fl, "r+");
    
    if(file == NULL) {
        perror("Page not found");
        return NULL;
    };

    free(fl);
    return file;
}





