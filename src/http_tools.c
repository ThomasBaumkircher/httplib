#include "http_tools.h"


char *read_file(char *path){
    // check if file exists
    struct stat buffer;
    if (stat(path, &buffer) == -1)
        return "FILE NOT FOUND";

    // open the file
    FILE *fp = fopen(path, "r");
    
    char *content = malloc(1);
    size_t i = 0;
    // read every character of the file until the end of file is reached
    for(char read = '\0'; read != EOF; i++) {
        read = fgetc(fp);
        content = (char *) realloc(content, i+1);
        content[i] = read;
    }
    // null-terminate the string (replace EOF character)
    content[i-1] = '\0';
    return content;
}



// Request functions

char *
get_request_type(char *requestBuffer)
{
    char *requestType = malloc(sizeof(char) * 10);
    
    char **requestLines = split_string(requestBuffer, "\r\n");
    char **requestFirstLine = split_string(requestLines[0], " ");

    strcpy(requestType, requestFirstLine[0]);

    free(requestLines);
    free(requestFirstLine);

    return requestType;
}

char *
get_http_version(char *requestBuffer)
{
    char *httpVersion = malloc(sizeof(char) * 10);
    
    char **requestLines = split_string(requestBuffer, "\r\n");
    char **requestFirstLine = split_string(requestLines[0], " ");

    strcpy(httpVersion, requestFirstLine[2]);

    free(requestLines);
    free(requestFirstLine);

    return httpVersion;
}

int
get_content_length(char *requestBuffer)
{
    int contentLength = 0;
    
    char **requestLines = split_string(requestBuffer, "\r\n");

    while (*requestLines != NULL) {
        if (strncmp(*requestLines, "Content-Length: ", 16) == 0) {
            contentLength = atoi((*requestLines)+16);
            break;
        }
        requestLines++;
    }

    return contentLength;
}

char *
get_content_type(char *requestBuffer)
{

    char *contentType = malloc(sizeof(char) * 100);
    
    char **requestLines = split_string(requestBuffer, "\r\n");

    while (*requestLines != NULL) {
        if (strncmp(*requestLines, "Content-Type: ", 14) == 0) {
            strcpy(contentType, (*requestLines)+14);
            break;
        }
        requestLines++;
    }
    return contentType;
}

char *
get_request_body(char *requestBuffer)
{
    char *requestBody = malloc(MAX_REQUEST_LEN);

    for (int i = 0; i < strlen(requestBuffer); i++) {
        if (strncmp(requestBuffer+i, "\r\n\r\n", 4) == 0) {
            strcpy(requestBody, requestBuffer+i+4);
            break;
        }
    }

    return requestBody;
}

char *
get_request_path(char *requestBuffer)
{
    char *requestPath = malloc(sizeof(char) * 100);
    
    char **requestLines = split_string(requestBuffer, "\r\n");
    char **requestFirstLine = split_string(requestLines[0], " ");

    strcpy(requestPath, requestFirstLine[1]);

    free(requestLines);
    free(requestFirstLine);

    return requestPath;
}

