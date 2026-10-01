#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "entry.h"
#include "utils.h"

void list_init(struct entry_list *list)
{
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

int list_add(struct entry_list *list, const char *name, const char *path)
{
    struct file_entry *e;
    struct stat st;

    /* lstat first, so a failing file is never added to the list. */
    if (lstat(path, &st) == -1) {
        fprintf(stderr, "myls: %s: %s\n", path, strerror(errno));
        return -1;
    }

    /* Grow the array when it is full (double the capacity). */
    if (list->count == list->capacity) {
        list->capacity = list->capacity == 0 ? 16 : list->capacity * 2;
        list->items = xrealloc(list->items,
                               list->capacity * sizeof(*list->items));
    }

    e = &list->items[list->count++];
    e->name = xstrdup(name);
    e->path = xstrdup(path);
    e->st = st;
    return 0;
}

void list_free(struct entry_list *list)
{
    size_t i;

    for (i = 0; i < list->count; i++) {
        free(list->items[i].name);
        free(list->items[i].path);
    }
    free(list->items);
    list_init(list);
}
