#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "print.h"

/* Number of decimal digits needed to print n. */
static int num_width(unsigned long long n)
{
    int w = 1;

    while (n >= 10) {
        n /= 10;
        w++;
    }
    return w;
}

/* Blocks used by a file, in units of 512 bytes (st_blocks already is). */
static unsigned long long entry_blocks(const struct file_entry *e)
{
    return (unsigned long long)e->st.st_blocks;
}

/* Return the -F indicator character for a file, or 0 if none. */
static char classify_char(const struct stat *st)
{
    if (S_ISDIR(st->st_mode))
        return '/';
    if (S_ISLNK(st->st_mode))
        return '@';
    if (S_ISSOCK(st->st_mode))
        return '=';
    if (S_ISFIFO(st->st_mode))
        return '|';
#ifdef S_ISWHT
    if (S_ISWHT(st->st_mode))
        return '%';
#endif
    if (st->st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))
        return '*';
    return 0;
}

void print_entries(const struct entry_list *list, const struct options *opts)
{
    size_t i;
    unsigned long long max_ino = 0, max_blk = 0;
    int ino_w, blk_w;
    char c;

    /* First pass: find the widest inode / block numbers for alignment. */
    for (i = 0; i < list->count; i++) {
        const struct file_entry *e = &list->items[i];

        if ((unsigned long long)e->st.st_ino > max_ino)
            max_ino = e->st.st_ino;
        if (entry_blocks(e) > max_blk)
            max_blk = entry_blocks(e);
    }
    ino_w = num_width(max_ino);
    blk_w = num_width(max_blk);

    /* Second pass: print each entry. */
    for (i = 0; i < list->count; i++) {
        const struct file_entry *e = &list->items[i];

        if (opts->inode)
            printf("%*llu ", ino_w, (unsigned long long)e->st.st_ino);
        if (opts->show_blocks)
            printf("%*llu ", blk_w, entry_blocks(e));
        printf("%s", e->name);
        if (opts->classify && (c = classify_char(&e->st)) != 0)
            putchar(c);
        putchar('\n');
    }
}
