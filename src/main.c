#include <stdio.h>

#include "entry.h"
#include "list.h"
#include "options.h"

int main(int argc, char *argv[])
{
    struct options opts;
    struct entry_list list;
    const char *dir;
    size_t i;

    int first = parse_options(argc, argv, &opts);

    dir = (first < argc) ? argv[first] : ".";

    list_init(&list);
    if (list_dir(dir, &opts, &list) == -1)
        return 1;

    /* Temporary: print names unsorted. Real printing comes in step 4. */
    for (i = 0; i < list.count; i++)
        printf("%s\n", list.items[i].name);

    list_free(&list);
    return 0;
}
