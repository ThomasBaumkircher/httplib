#include "strings.h"

char **split_string(const char *str, const char *delim) {
    // allocate space for the splitted string
    size_t n = 0;
    size_t splitted_size = SPLIT_STRING_SLICES_BASE;
    char **splitted = (char **) malloc(sizeof(char *) * splitted_size);
    char *token = strtok(strdup(str), delim);

    while(token != NULL) {
        if(n >= splitted_size) {
            splitted = (char **) realloc(splitted, sizeof(char *) * splitted_size * 2);
            splitted_size *= 2;
        }

        splitted[n] = (char *) malloc(strlen(token) + 1);
        strcpy(splitted[n], token);
        token = strtok(NULL, delim);
        n++;
    }

    splitted = (char **) realloc(splitted, sizeof(char *) * n+1);
    splitted[n] = NULL;

    return splitted;
}
