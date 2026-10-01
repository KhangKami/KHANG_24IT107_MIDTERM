#include <ctype.h>
#include <grp.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "format.h"
#include "print.h"
#include "utils.h"

/* Pre-formatted text of every column for one entry. */
struct row {
    char ino[64];
    char blk[64];
    char mode[12];
    char links[64];
    char owner[64];
    char group[64];
    char size[64];
    char date[64];
};

/* Fill every column of one row from an entry. */
static void fill_row(struct row *r, const struct file_entry *e,
                     const struct options *opts)
{
    const struct stat *st = &e->st;
    struct passwd *pw = NULL;
    struct group *gr = NULL;
    time_t t, now;
    struct tm *tm;

    snprintf(r->ino, sizeof(r->ino), "%llu", (unsigned long long)st->st_ino);
    fmt_blocks(r->blk, sizeof(r->blk), (unsigned long long)st->st_blocks, opts);
    fmt_mode(r->mode, st->st_mode);
    snprintf(r->links, sizeof(r->links), "%lu", (unsigned long)st->st_nlink);

    /* Owner and group: name if known and -n not given, else the number. */
    if (!opts->numeric_ids) {
        pw = getpwuid(st->st_uid);
        gr = getgrgid(st->st_gid);
    }
    if (pw != NULL)
        snprintf(r->owner, sizeof(r->owner), "%s", pw->pw_name);
    else
        snprintf(r->owner, sizeof(r->owner), "%lu", (unsigned long)st->st_uid);
    if (gr != NULL)
        snprintf(r->group, sizeof(r->group), "%s", gr->gr_name);
    else
        snprintf(r->group, sizeof(r->group), "%lu", (unsigned long)st->st_gid);

    /* Size: major, minor for devices; bytes (or human readable) otherwise. */
    if (S_ISCHR(st->st_mode) || S_ISBLK(st->st_mode))
        snprintf(r->size, sizeof(r->size), "%lu, %lu",
                 (unsigned long)major(st->st_rdev),
                 (unsigned long)minor(st->st_rdev));
    else if (opts->size_mode == SIZE_HUMAN)
        fmt_human(r->size, sizeof(r->size), (unsigned long long)st->st_size);
    else
        snprintf(r->size, sizeof(r->size), "%llu",
                 (unsigned long long)st->st_size);

    /* Date: time of day for recent files, year for old or future ones. */
    switch (opts->time_kind) {
    case TIME_CTIME: t = st->st_ctime; break;
    case TIME_ATIME: t = st->st_atime; break;
    default:         t = st->st_mtime; break;
    }
    now = time(NULL);
    tm = localtime(&t);
    if (tm == NULL)
        snprintf(r->date, sizeof(r->date), "?");
    else if (t > now || now - t > 15724800)      /* about six months */
        strftime(r->date, sizeof(r->date), "%b %e  %Y", tm);
    else
        strftime(r->date, sizeof(r->date), "%b %e %H:%M", tm);
}

/* Keep the maximum string length seen so far. */
static void widen(int *w, const char *s)
{
    int len = (int)strlen(s);

    if (len > *w)
        *w = len;
}

/* Print a file name, replacing non-printable characters with '?' when
 * -q is active (or by default on a terminal). */
static void print_name(const char *name, const struct options *opts)
{
    int force_q;
    const unsigned char *p;

    if (opts->nonprint == NP_QUESTION)
        force_q = 1;
    else if (opts->nonprint == NP_RAW)
        force_q = 0;
    else
        force_q = isatty(STDOUT_FILENO);

    for (p = (const unsigned char *)name; *p; p++) {
        if (force_q && !isprint(*p))
            putchar('?');
        else
            putchar(*p);
    }
}

void print_entries(const struct entry_list *list, const struct options *opts,
                   int show_total)
{
    struct row *rows;
    int w_ino = 0, w_blk = 0, w_links = 0, w_owner = 0, w_group = 0;
    int w_size = 0;
    unsigned long long total = 0;
    char buf[64];
    size_t i;
    char c;

    if (list->count == 0)
        return;

    /* "total" line: always for -l, for -s only on a terminal. */
    if (show_total && (opts->long_format ||
                       (opts->show_blocks && isatty(STDOUT_FILENO)))) {
        for (i = 0; i < list->count; i++)
            total += (unsigned long long)list->items[i].st.st_blocks;
        fmt_blocks(buf, sizeof(buf), total, opts);
        printf("total %s\n", buf);
    }


    rows = xmalloc(list->count * sizeof(*rows));

    /* Pass 1: format every column and measure the widths. */
    for (i = 0; i < list->count; i++) {
        fill_row(&rows[i], &list->items[i], opts);
        widen(&w_ino, rows[i].ino);
        widen(&w_blk, rows[i].blk);
        widen(&w_links, rows[i].links);
        widen(&w_owner, rows[i].owner);
        widen(&w_group, rows[i].group);
        widen(&w_size, rows[i].size);
    }

    /* Pass 2: print. */
    for (i = 0; i < list->count; i++) {
        const struct file_entry *e = &list->items[i];
        const struct row *r = &rows[i];

        if (opts->inode)
            printf("%*s ", w_ino, r->ino);
        if (opts->show_blocks)
            printf("%*s ", w_blk, r->blk);
        if (opts->long_format)
            printf("%s  %*s %-*s  %-*s  %*s %s ", r->mode, w_links, r->links,
                   w_owner, r->owner, w_group, r->group, w_size, r->size,
                   r->date);

        print_name(e->name, opts);
        if (opts->classify && (c = classify_char(&e->st)) != 0)
            putchar(c);

        /* Symbolic link: show "-> target". */
        if (opts->long_format && S_ISLNK(e->st.st_mode)) {
            char target[PATH_MAX];
            ssize_t n = readlink(e->path, target, sizeof(target) - 1);

            if (n >= 0) {
                target[n] = '\0';
                printf(" -> %s", target);
            }
        }
        putchar('\n');
    }
    free(rows);
}
