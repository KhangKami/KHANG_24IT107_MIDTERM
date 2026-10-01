#include <stdio.h>

#include "print.h"

void print_entries(const struct entry_list *list, const struct options *opts)
{
    size_t i;

    (void)opts;     /* used by later steps (-l, -i, -s, -F ...) */
    for (i = 0; i < list->count; i++)
        printf("%s\n", list->items[i].name);
}
