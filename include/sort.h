#ifndef SORT_H
#define SORT_H

#include "entry.h"
#include "options.h"

/*
 * Sort the list in place according to the options:
 * default by name, -t by time, -S by size, -r reverses, -f disables.
 */
void sort_entries(struct entry_list *list, const struct options *opts);

#endif
