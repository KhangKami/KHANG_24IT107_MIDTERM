#ifndef PRINT_H
#define PRINT_H

#include "entry.h"
#include "options.h"

/* Print every entry of the list, one per line. */
void print_entries(const struct entry_list *list, const struct options *opts);

#endif
