#pragma once


#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "http_tools.h"
#include "cstrings.h"


#define ROUTER_HANDLES_SIZE_BASE 4
#define MAX_REQUEST_SIZE 2048
#define SLUG_LABEL_BEGIN '<'
#define SLUG_LABEL_END '>'
#define SLUG_MAX_LEN 8

#define THREAD_SLEEP_MILLIS 5


// Request
typedef struct Request {
    // Headers
    char *method;
    char *httpVersion;
    int contentLength;
    char *contentType;
    char *path;

    // Path slugs
    void *slugs;

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
typedef struct RequestSlug {
    char *slugName;
    int pathInd;
} HttplibSlug;

typedef struct RequestHandle {
    char *path;

    HttplibSlug *slugs;
    int slugsSize;

    httplib_handlefunc *func;
} HttplibRequestHandle;

// Router
typedef struct Router {
    struct sockaddr_in hostAddr;

    HttplibRequestHandle *handles;
    int handlesCount;
    int handlesSize;
} HttplibRouter;

// Threading fd queue
typedef struct FDqueue HttplibFDqueue;
typedef struct FDqueue {
  int fd;

  HttplibFDqueue *prev;
} HttplibFDqueue;


// Library functions

// Instantiator of the router
HttplibRouter *httplib_instantiate(int);
// Destructor of the router
void httplib_destroy(HttplibRouter *);
// Add a handle function to the router
void httplib_add_handlefunc(HttplibRouter *, const char *, httplib_handlefunc);
// Add a static file to the router
void httplib_add_static(HttplibRouter *, const char *);
// Main loop of the library, handling the requests
int httplib_serve(HttplibRouter *, int);

// Request functions
HttplibRequest *httplib_request_parse(char *);
void httplib_request_destroy(HttplibRequest *);

// Response functions
HttplibResponseWriter *httplib_responsewriter_instantiate(int);
void httplib_responsewriter_destroy(HttplibResponseWriter *);
int httplib_find_handle(HttplibRequest *request, HttplibRequestHandle *handles, int handlesCount);
int httplib_match_path(const char *path, const char *handlePath);
void httplib_responsewriter_set_header(HttplibResponseWriter *, const char *, const char *);
void httplib_write_response(HttplibResponseWriter *responseWriter, int statusCode, const char *statusText, const char *contentType, const char *body);

// Threading functions
void *thread_idle(void *);
void handle_conn(int, HttplibRouter *);

