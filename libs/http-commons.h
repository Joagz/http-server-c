#include <stdio.h>

#define PAGES_FOLDER   "./pages"
#define BUF_SIZE       4096
#define PORT           80
#define MAX_REQUEST_SIZE 512

// Read the file and return its information
char * read_page_bytes(FILE * fp);

// Return file descriptor for the file in req. Else throw -1
FILE * find_page(char * req);



