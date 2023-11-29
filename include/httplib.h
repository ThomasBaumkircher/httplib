#pragma once


#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>

#include "strings.h"
#include "http_tools.h"


#define ROUTER_HANDLES_SIZE_BASE 4
#define MAX_REQUEST_SIZE 2048


// Request
typedef struct Request {
    // Headers
    char *method;
    char *route;
    char *httpVersion;
    int contentLength;
    char *contentType;
    char *path;

    // Body
    char *body;
} HttplibRequest;

// Response writer
typedef struct ResponseWriter {
    int responseFD;
    char *contentType;
    char *resBody;
    char **resHeaders;
} HttplibResponseWriter;

// Handle function template
typedef int httplib_handlefunc(HttplibRequest *, HttplibResponseWriter *);

// Request Handle
typedef struct RequestHandle {
    char *path;
    httplib_handlefunc *func;
} HttplibRequestHandle;

// Router
typedef struct Router {
    struct sockaddr_in hostAddr;

    HttplibRequestHandle *handles;
    int handlesCount;
    int handlesSize;
} HttplibRouter;


// Library functions

// Instantiator of the router
HttplibRouter *httplib_instantiate();
// Destructor of the router
void httplib_destroy(HttplibRouter *);
// Add a handle function to the router
void httplib_add_handlefunc(HttplibRouter *, char *, httplib_handlefunc);
// Add a static file to the router
void httplib_add_static(HttplibRouter *, char *);
// Main loop of the library, handling the requests
int httplib_serve(HttplibRouter *, int);

// Request functions
HttplibRequest *httplib_request_parse(char *);
void httplib_request_destroy(HttplibRequest *);

// Response functions
HttplibResponseWriter *httplib_responsewriter_instantiate(int);
void httplib_responsewriter_destroy(HttplibResponseWriter *);
int httplib_find_handle(HttplibRequest *request, HttplibRequestHandle *handles, int handlesCount);
void httplib_responsewriter_set_header(HttplibResponseWriter *, char *, char *);
void httplib_write_response(HttplibResponseWriter *responseWriter, int statusCode, char *statusText, char *contentType, char *body);