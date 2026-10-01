#ifndef OPTIONS_H
#define OPTIONS_H

/* Which timestamp is used for sorting (-t) and printing (-l). */
enum time_kind {
    TIME_MTIME,   /* default: last modification */
    TIME_CTIME,   /* -c: last status change     */
    TIME_ATIME    /* -u: last access            */
};

/* How non-printable characters in names are shown. */
enum nonprint_mode {
    NP_AUTO,      /* default: '?' if stdout is a terminal, raw otherwise */
    NP_QUESTION,  /* -q */
    NP_RAW        /* -w */
};

/* Size display unit for -s and -l. */
enum size_mode {
    SIZE_DEFAULT, /* blocks of 512 bytes (or BLOCKSIZE) / plain bytes */
    SIZE_KILO,    /* -k */
    SIZE_HUMAN    /* -h */
};

/* All command-line flags, filled in by parse_options(). */
struct options {
    int all;            /* -a */
    int almost_all;     /* -A */
    int dir_as_file;    /* -d */
    int classify;       /* -F */
    int unsorted;       /* -f */
    int inode;          /* -i */
    int long_format;    /* -l or -n */
    int numeric_ids;    /* -n */
    int recursive;      /* -R */
    int reverse;        /* -r */
    int sort_size;      /* -S */
    int show_blocks;    /* -s */
    int sort_time;      /* -t */
    enum time_kind time_kind;
    enum nonprint_mode nonprint;
    enum size_mode size_mode;
};

/*
 * Parse argv. Fills *opts and returns the index of the first
 * non-option argument (the first file operand).
 * Prints usage and exits with status 1 on an invalid option.
 */
int parse_options(int argc, char *argv[], struct options *opts);

#endif
