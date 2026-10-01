#include <stdio.h>
#include <stdlib.h>

#include "format.h"

void fmt_human(char *buf, size_t n, unsigned long long bytes)
{
    static const char units[] = "BKMGTP";
    double v = (double)bytes;
    int i = 0;

    while (v >= 1024.0 && i < 5) {
        v /= 1024.0;
        i++;
    }
    if (i == 0)
        snprintf(buf, n, "%lluB", bytes);
    else if (v < 10.0)
        snprintf(buf, n, "%.1f%c", v, units[i]);
    else
        snprintf(buf, n, "%.0f%c", v, units[i]);
}

/*
 * Block size from the BLOCKSIZE environment variable: a number with an
 * optional k/m/g suffix. Invalid or missing values give 512; values
 * below 512 are raised to 512.
 */
static unsigned long long block_unit(void)
{
    const char *env = getenv("BLOCKSIZE");
    char *end;
    unsigned long long n;

    if (env == NULL || *env == '\0')
        return 512;
    n = strtoull(env, &end, 10);
    if (end == env)
        return 512;
    switch (*end) {
    case 'k': case 'K': n *= 1024ULL; break;
    case 'm': case 'M': n *= 1024ULL * 1024; break;
    case 'g': case 'G': n *= 1024ULL * 1024 * 1024; break;
    case '\0': break;
    default: return 512;
    }
    if (n < 512)
        n = 512;
    return n;
}

void fmt_blocks(char *buf, size_t n, unsigned long long blocks,
                const struct options *opts)
{
    unsigned long long bytes = blocks * 512ULL;
    unsigned long long unit;

    if (opts->size_mode == SIZE_HUMAN) {
        fmt_human(buf, n, bytes);
        return;
    }
    /* -k wins over BLOCKSIZE; otherwise use the environment (or 512). */
    unit = (opts->size_mode == SIZE_KILO) ? 1024ULL : block_unit();
    snprintf(buf, n, "%llu", (bytes + unit - 1) / unit);
}

void fmt_mode(char *out, mode_t m)
{
    char type = '-';

    if (S_ISDIR(m)) type = 'd';
    else if (S_ISLNK(m)) type = 'l';
    else if (S_ISBLK(m)) type = 'b';
    else if (S_ISCHR(m)) type = 'c';
    else if (S_ISSOCK(m)) type = 's';
    else if (S_ISFIFO(m)) type = 'p';
#ifdef S_ISWHT
    else if (S_ISWHT(m)) type = 'w';
#endif

    out[0] = type;
    out[1] = (m & S_IRUSR) ? 'r' : '-';
    out[2] = (m & S_IWUSR) ? 'w' : '-';
    if (m & S_ISUID)
        out[3] = (m & S_IXUSR) ? 's' : 'S';
    else
        out[3] = (m & S_IXUSR) ? 'x' : '-';
    out[4] = (m & S_IRGRP) ? 'r' : '-';
    out[5] = (m & S_IWGRP) ? 'w' : '-';
    if (m & S_ISGID)
        out[6] = (m & S_IXGRP) ? 's' : 'S';
    else
        out[6] = (m & S_IXGRP) ? 'x' : '-';
    out[7] = (m & S_IROTH) ? 'r' : '-';
    out[8] = (m & S_IWOTH) ? 'w' : '-';
    if (m & S_ISVTX)
        out[9] = (m & S_IXOTH) ? 't' : 'T';
    else
        out[9] = (m & S_IXOTH) ? 'x' : '-';
    out[10] = '\0';
}

char classify_char(const struct stat *st)
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
