#ifndef SERVER_H
#define SERVER_H


#include <arpa/inet.h>
#include <errno.h>
#include <sys/socket.h>

#include "strings.h"
#include "http_tools.h"


#define PORT 8080
#define MAX_REQUEST_SIZE 2048


// constant return headers
const char *text_header_html_c = "HTTP/1.0 200 OK\nContent-Type: text/html\nContent-Length: \0";
const char *text_header_css_c = "HTTP/1.0 200 OK\nContent-Type: text/css\nContent-Length: \0";
const char *text_header_js_c = "HTTP/1.0 200 OK\nContent-Type: text/javascript\nContent-Length: \0";
const char *text_header_plain_c = "HTTP/1.0 200 OK\nContent-Type: text/plain\nContent-Length: \0";
const char *text_header_json_c = "HTTP/1.0 200 OK\nContent-Type: application/json\nContent-Length: \0";
const char *not_found_response_c = "HTTP/1.0 404 Not Found\nContent-Type: text/html\nContent-Length: 0 \0";


// Other library functions
char *request_handle(char *);
char *html_handle(char *);
char *api_handle(char *);


#endif