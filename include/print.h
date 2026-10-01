#ifndef PRINT_H
#define PRINT_H

#include "entry.h"
#include "options.h"

/*
 * Print every entry of the list (short or long format).
 * show_total: print the "total N" line (used for directory contents).
 */
void print_entries(const struct entry_list *list, const struct options *opts,
                   int show_total);

#endif
