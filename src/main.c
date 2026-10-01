#include <stdio.h>

#include "options.h"

int main(int argc, char *argv[])
{
    struct options opts;
    int first;
    int i;

    first = parse_options(argc, argv, &opts);

    /* Temporary debug output: will be removed in later steps. */
    printf("all=%d almost_all=%d long=%d numeric=%d recursive=%d reverse=%d\n",
           opts.all, opts.almost_all, opts.long_format,
           opts.numeric_ids, opts.recursive, opts.reverse);
    printf("time_kind=%d nonprint=%d size_mode=%d\n",
           opts.time_kind, opts.nonprint, opts.size_mode);
    for (i = first; i < argc; i++)
        printf("operand: %s\n", argv[i]);
    return 0;
}
