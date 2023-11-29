#include "httplib.h"

HttplibRouter *
httplib_instantiate()
{
    HttplibRouter *router = malloc(sizeof(HttplibRouter));

    router->hostAddr.sin_family = AF_INET;
    router->hostAddr.sin_addr.s_addr = INADDR_ANY;

    router->handles = NULL;
    router->handlesSize = 0;
    router->handlesCount = 0;

    return router;
}

void
httplib_destroy(HttplibRouter *router)
{
    if (router->handles != NULL)
        free(router->handles);

    if (router != NULL)
        free(router);
}

void
httplib_add_handlefunc(HttplibRouter *router, char *path, httplib_handlefunc func)
{
    if (router->handles == NULL) {
        router->handles = malloc(sizeof(HttplibRequestHandle) * ROUTER_HANDLES_SIZE_BASE);
        router->handlesSize = ROUTER_HANDLES_SIZE_BASE;
    }
    else if (router->handlesSize == router->handlesCount) {
        router->handles = realloc(router->handles, sizeof(HttplibRequestHandle) * router->handlesSize * 2);
        router->handlesSize *= 2;
    }

    router->handles[router->handlesCount].path = path;
    router->handles[router->handlesCount].func = func;
    router->handlesCount++;
}

int
httplib_handle_static(HttplibRequest *request, HttplibResponseWriter *response)
{
    // Read in the index.html file
    char *path = request->path;
    if (path[0] == '/')
        path++;

    char *index_html = read_file(path);

    // Write the index.html file to the response
    httplib_write_response(response, 200, "OK", "text/html", index_html);

    return 0;
}

void
httplib_add_static(HttplibRouter *router, char *path)
{
    httplib_add_handlefunc(router, path, httplib_handle_static);
}

int
httplib_serve(HttplibRouter *router, int port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
    perror("webserver (socket)");
    return 1;
    }
    printf("socket created successfully\n");

    router->hostAddr.sin_port = htons(port);
    int host_addrlen = sizeof(router->hostAddr);

    // Allow the socket to be reused
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) <
        0) {
    perror("webserver (setsockopt)");
    return 1;
    }

    // Bind the socket to the addressocket created successfu
    if (bind(sockfd, (struct sockaddr *)&(router->hostAddr), host_addrlen) != 0) {
    perror("webserver (bind)");
    while (bind(sockfd, (struct sockaddr *)&(router->hostAddr), host_addrlen) != 0)
        ;
    }
    printf("socket successfully bound to address\n");

    // Listen for incoming connections
    if (listen(sockfd, SOMAXCONN) != 0) {
    perror("webserver (listen)");
    return 1;
    }
    printf("server listening for connections\n");

    // Handle connections
    for (;;) {
        // Accept incoming connections
        int newsockfd = accept(sockfd, (struct sockaddr *)&(router->hostAddr),
                             (socklen_t *)&host_addrlen);
        if (newsockfd < 0) {
            perror("webserver (accept)");
            continue;
        }
        printf("\n\nconnection accepted\n");
    
        // Read the request
        char *buffer = (char *)malloc(MAX_REQUEST_SIZE);
        int readBytes = read(newsockfd, buffer, MAX_REQUEST_SIZE);
        if (readBytes < 0) {
            perror("webserver (read)");
            continue;
        }
    
        // Parse the request
        HttplibRequest *request = httplib_request_parse(buffer);
    
        // Write the response
        HttplibResponseWriter *responseWriter = httplib_responsewriter_instantiate(newsockfd);
    
        // Find the handle function
        int handleIndex = httplib_find_handle(request, router->handles, router->handlesCount);
        if (handleIndex == -1) {
            printf("handle not found\n");
            httplib_write_response(responseWriter, 404, "Not Found", "text/plain", "404 Not Found");
            printf("response written successfully\n");
            goto sock_close;
        }
        printf("handle found\n");
    
        // Call the handle function
        int handleResult = router->handles[handleIndex].func(request, responseWriter);
        printf("handle function called successfully with return code: %d\n", handleResult);
    
        sock_close:
        // Free the request
        httplib_request_destroy(request);
    
        // Free the response writer
        httplib_responsewriter_destroy(responseWriter);
    
        // Free the buffer
        free(buffer);
    
        // Close the connection
        close(newsockfd);
        printf("connection closed\n");
    }
}

HttplibRequest *
httplib_request_parse(char *requestBuffer)
{
    HttplibRequest *request = malloc(sizeof(HttplibRequest));

    // Get the request type
    request->method = get_request_type(requestBuffer);

    // Get the routes
    request->route = get_route(requestBuffer);

    // Get the http version
    request->httpVersion = get_http_version(requestBuffer);

    // Get the content length
    request->contentLength = get_content_length(requestBuffer);

    // Get the content type
    request->contentType = get_content_type(requestBuffer);

    // Get the request body
    request->body = get_request_body(requestBuffer);

    // Get the path
    request->path = get_request_path(requestBuffer);

    return request;
}

void
httplib_request_destroy(HttplibRequest *request)
{
    if (request->method != NULL)
        free(request->method);

    if (request->route != NULL)
        free(request->route);

    if (request->httpVersion != NULL)
        free(request->httpVersion);

    if (request->contentType != NULL)
        free(request->contentType);

    if (request->body != NULL)
        free(request->body);

    if (request != NULL)
        free(request);

    if (request->path != NULL)
        free(request->path);
}

HttplibResponseWriter *
httplib_responsewriter_instantiate(int responseFD)
{
    HttplibResponseWriter *responseWriter = malloc(sizeof(HttplibResponseWriter));

    responseWriter->responseFD = responseFD;
    responseWriter->contentType = NULL;
    responseWriter->resBody = NULL;
    responseWriter->resHeaders = NULL;

    return responseWriter;
}

void
httplib_responsewriter_destroy(HttplibResponseWriter *responseWriter)
{
    if (responseWriter->contentType != NULL)
        free(responseWriter->contentType);

    if (responseWriter->resBody != NULL)
        free(responseWriter->resBody);

    if (responseWriter->resHeaders != NULL)
        free(responseWriter->resHeaders);

    if (responseWriter != NULL)
        free(responseWriter);
}

int
httplib_find_handle(HttplibRequest *request, HttplibRequestHandle *handles, int handlesCount)
{
    for (int i = 0; i < handlesCount; i++) {
        if (strcmp(handles[i].path, request->route) == 0)
            return i;
    }

    return -1;
}

void
httplib_responsewriter_set_header(HttplibResponseWriter *responseWriter, char *headerName, char *headerValue)
{
    if (responseWriter->resHeaders == NULL) {
        responseWriter->resHeaders = malloc(sizeof(char *) * 1);
        responseWriter->resHeaders[0] = malloc(sizeof(char) * 100);
        sprintf(responseWriter->resHeaders[0], "%s: %s\r\n", headerName, headerValue);
    }
    else {
        responseWriter->resHeaders = realloc(responseWriter->resHeaders, sizeof(char *) * (sizeof(responseWriter->resHeaders) + 1));
        responseWriter->resHeaders[sizeof(responseWriter->resHeaders)] = malloc(sizeof(char) * 100);
        sprintf(responseWriter->resHeaders[sizeof(responseWriter->resHeaders)], "%s: %s\r\n", headerName, headerValue);
    }
}

void
httplib_write_response(HttplibResponseWriter *responseWriter, int statusCode, char *statusText, char *contentType, char *body)
{
    // Write the status line
    char *statusLine = malloc(sizeof(char) * 100);
    sprintf(statusLine, "HTTP/1.1 %d %s\r\n", statusCode, statusText);
    write(responseWriter->responseFD, statusLine, strlen(statusLine));

    // Write the content type
    char *contentTypeHeader = malloc(sizeof(char) * 100);
    sprintf(contentTypeHeader, "Content-Type: %s\r\n", contentType);
    write(responseWriter->responseFD, contentTypeHeader, strlen(contentTypeHeader));

    // Write the content length
    char *contentLengthHeader = malloc(sizeof(char) * 100);
    sprintf(contentLengthHeader, "Content-Length: %ld\r\n", strlen(body));
    write(responseWriter->responseFD, contentLengthHeader, strlen(contentLengthHeader));

    // Write the body
    write(responseWriter->responseFD, "\r\n", 2);
    write(responseWriter->responseFD, body, strlen(body));
}