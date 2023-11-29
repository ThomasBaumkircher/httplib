#include "../include/httplib.h"


int handle_root(HttplibRequest *, HttplibResponseWriter *);
int handle_test(HttplibRequest *, HttplibResponseWriter *);

int
main(int argc, char **argv)
{
    HttplibRouter *router = httplib_instantiate();

    httplib_add_handlefunc(router, "/", handle_root);
    httplib_add_handlefunc(router, "/test", handle_test);

    httplib_serve(router, (argc > 1) ? atoi(argv[1]) : 8080);

    httplib_destroy(router);

    return 0;
}


int
handle_root(HttplibRequest *request, HttplibResponseWriter *response)
{
    httplib_write_response(response, 200, "OK", "text/html", "<h1>root</h1>");
    return 0;
}

int
handle_test(HttplibRequest *request, HttplibResponseWriter *response)
{
    httplib_write_response(response, 200, "OK", "text/html", "<h1>test</h1>");
    return 0;
}