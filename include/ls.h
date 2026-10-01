#ifndef LS_H
#define LS_H

#include "options.h"

/*
 * Run the listing for the given operands (files and directories).
 * Non-directories are shown first, then each directory (recursively
 * with -R). Returns the exit status: 0 on success, 1 on any error.
 */
int ls_run(int count, char *names[], const struct options *opts);

#endif
