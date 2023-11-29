#include "../include/api.h"

char **api_get_routes(char *request) {
    // split the request into it's lines
    char **request_lines = split_string(request, "\n");
    // split the first line into it's parts
    char **page_header_splitted = split_string(request_lines[0], " ");
    // split the route into it's parts
    char **page_splitted = split_string(page_header_splitted[1], "/");

    return page_splitted + 1;
}