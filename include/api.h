#ifndef API_H
#define API_H


#include <stdlib.h>
#include <sqlite3.h>

#include "strings.h"
#include "http_tools.h"


char **api_get_routes(char *);


#endif