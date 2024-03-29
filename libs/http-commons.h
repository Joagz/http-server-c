#include <stdio.h>

#ifndef DEF_HTTPRESPONSE
    #define DEF_HTTPRESPONSE "HTTP/1.1 200 OK\r\n\n\r\n"
#endif // !DEF_HTTPRESPONSE

#define RESOURCES_DIR   "./resources"
#define BUF_SIZE       4096
#define PORT           80
#define MAX_REQUEST_SIZE 512

// Read the file and return its information
char * read_page_bytes(FILE * fp);

// Return file descriptor for the file in req. Else throw -1
FILE * find_page(char * req);

char * generateHttpResponse(char * fileToSend, char * fileData);


