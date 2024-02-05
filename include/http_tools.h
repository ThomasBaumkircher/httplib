#ifndef HTML_TOOLS_H
#define HTML_TOOLS_H


#define HTTP_SEND_BYTES_CORRECTION_SUB 0
#define MAX_RESPONSE_LEN_DIGITS 4
#define MAX_RESPONSE_LEN 1024


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

<<<<<<< HEAD
#include "strings.h"
#include "api.h"
=======
#include "cstrings.h"
>>>>>>> 189e4e51b651e103dbf33ac9ca700f1f1962ee5e


#define MAX_REQUEST_LEN 2048


char *read_file(char *);
char *get_request_type(char *);
<<<<<<< HEAD
char *get_route(char *);
=======
>>>>>>> 189e4e51b651e103dbf33ac9ca700f1f1962ee5e
char *get_http_version(char *);
int get_content_length(char *);
char *get_content_type(char *);
char *get_request_body(char *);
<<<<<<< HEAD


#endif
=======
char *get_request_path(char *);


#endif

>>>>>>> 189e4e51b651e103dbf33ac9ca700f1f1962ee5e
