#ifndef LIST_H
#define LIST_H

#include "entry.h"
#include "options.h"

/*
 * Read all entries of directory `dir` into `out`.
 * Names starting with '.' are skipped unless -a or -A is set;
 * "." and ".." are only kept when -a is set.
 * Returns 0 on success, -1 if the directory cannot be opened
 * (an error message is printed).
 */
int list_dir(const char *dir, const struct options *opts,
             struct entry_list *out);

#endif
