#include "../include/httplib.h"


int handle_root(HttplibRequest *, HttplibResponseWriter *);
int handle_test(HttplibRequest *, HttplibResponseWriter *);

int
main(int argc, char **argv)
{
    HttplibRouter *router = httplib_instantiate(1);

    httplib_add_handlefunc(router, "/", handle_root);
    httplib_add_handlefunc(router, "/test/<int>", handle_test);

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
    struct slugs {
        int id;
    } *slugs = request->slugs;

    char *res_text = malloc(100);
    sprintf(res_text, "<h1>test %d</h1>", slugs->id);

    httplib_write_response(response, 200, "OK", "text/html", res_text);
    free(res_text);

    return 0;
}
