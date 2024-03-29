#ifndef DEF_HTTPRESPONSE
    #define DEF_HTTPRESPONSE "HTTP/1.1 200 OK\r\nHello World\r\n\r\n"
#endif // !DEF_HTTPRESPONSE

#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"

struct http_req {
    char * method;
    char * url;
};

// gets a raw HTTP request message and converts it to struct http_req 
struct http_req filterHttpRequest(char * req);

// returns the index of the handler interface
int filterHandler(struct http_req req);



