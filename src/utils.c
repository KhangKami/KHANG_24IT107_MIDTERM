#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

void *xmalloc(size_t size)
{
    void *p = malloc(size);

    if (p == NULL) {
        fprintf(stderr, "myls: out of memory\n");
        exit(1);
    }
    return p;
}

void *xrealloc(void *ptr, size_t size)
{
    void *p = realloc(ptr, size);

    if (p == NULL) {
        fprintf(stderr, "myls: out of memory\n");
        exit(1);
    }
    return p;
}

char *xstrdup(const char *s)
{
    char *p = strdup(s);

    if (p == NULL) {
        fprintf(stderr, "myls: out of memory\n");
        exit(1);
    }
    return p;
}
