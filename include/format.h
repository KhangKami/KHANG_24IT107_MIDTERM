#ifndef FORMAT_H
#define FORMAT_H

#include <stddef.h>
#include <sys/stat.h>

#include "options.h"

/* Format a byte count as 1.5K, 23M ... (used by -h). */
void fmt_human(char *buf, size_t n, unsigned long long bytes);

/*
 * Format a block count (st_blocks, 512-byte units) according to the
 * options: -h human readable, -k 1024-byte units, otherwise 512 bytes
 * or the BLOCKSIZE environment variable. Partial units round up.
 */
void fmt_blocks(char *buf, size_t n, unsigned long long blocks,
                const struct options *opts);

/* Build the 10-character mode string such as "drwxr-xr-t" (out >= 11). */
void fmt_mode(char *out, mode_t m);

/* Return the -F indicator character for a file, or 0 if none. */
char classify_char(const struct stat *st);

#endif
