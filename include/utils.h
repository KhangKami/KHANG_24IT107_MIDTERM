#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

/* malloc/realloc/strdup wrappers: print an error and exit on failure. */
void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);
char *xstrdup(const char *s);

#endif
