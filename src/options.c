#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "options.h"

/* Print a usage message and exit with failure status. */
static void usage(void)
{
    fprintf(stderr, "usage: myls [-AacdFfhiklnqRrSstuw] [file ...]\n");
    exit(1);
}

int parse_options(int argc, char *argv[], struct options *opts)
{
    int ch;

    /* Start from all-zero defaults. */
    memset(opts, 0, sizeof(*opts));
    opts->time_kind = TIME_MTIME;
    opts->nonprint = NP_AUTO;
    opts->size_mode = SIZE_DEFAULT;

    while ((ch = getopt(argc, argv, "AacdFfhiklnqRrSstuw")) != -1) {
        switch (ch) {
        case 'A': opts->almost_all = 1; break;
        case 'a': opts->all = 1; break;
        case 'c': opts->time_kind = TIME_CTIME; break;
        case 'd': opts->dir_as_file = 1; break;
        case 'F': opts->classify = 1; break;
        case 'f': opts->unsorted = 1; break;
        case 'h': opts->size_mode = SIZE_HUMAN; break;
        case 'i': opts->inode = 1; break;
        case 'k': opts->size_mode = SIZE_KILO; break;
        case 'l':
            opts->long_format = 1;
            opts->numeric_ids = 0;   /* -l and -n override each other */
            break;
        case 'n':
            opts->long_format = 1;
            opts->numeric_ids = 1;
            break;
        case 'q': opts->nonprint = NP_QUESTION; break;
        case 'R': opts->recursive = 1; break;
        case 'r': opts->reverse = 1; break;
        case 'S': opts->sort_size = 1; break;
        case 's': opts->show_blocks = 1; break;
        case 't': opts->sort_time = 1; break;
        case 'u': opts->time_kind = TIME_ATIME; break;
        case 'w': opts->nonprint = NP_RAW; break;
        default:
            usage();
        }
    }
    return optind;
}
