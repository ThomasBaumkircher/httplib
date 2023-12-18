#ifndef HTML_TOOLS_H
#define HTML_TOOLS_H


#define HTTP_SEND_BYTES_CORRECTION_SUB 0
#define MAX_RESPONSE_LEN_DIGITS 4
#define MAX_RESPONSE_LEN 1024


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "cstrings.h"


#define MAX_REQUEST_LEN 2048


char *read_file(char *);
char *get_request_type(char *);
char *get_http_version(char *);
int get_content_length(char *);
char *get_content_type(char *);
char *get_request_body(char *);
char *get_request_path(char *);


#endif

