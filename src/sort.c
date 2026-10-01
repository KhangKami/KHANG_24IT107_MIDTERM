#include <stdlib.h>
#include <string.h>

#include "sort.h"

/* qsort() comparators cannot take extra arguments, so keep the active
 * options in a file-scope pointer while sorting. */
static const struct options *cur_opts;

/* Pick the timestamp selected by -c / -u (default: modification time). */
static time_t entry_time(const struct file_entry *e)
{
    switch (cur_opts->time_kind) {
    case TIME_CTIME: return e->st.st_ctime;
    case TIME_ATIME: return e->st.st_atime;
    default:         return e->st.st_mtime;
    }
}

/* Compare by name (lexicographic order). */
static int cmp_name(const struct file_entry *a, const struct file_entry *b)
{
    return strcmp(a->name, b->name);
}

/* Main comparator: primary key from -S / -t, then name as tie-breaker. */
static int compare(const void *pa, const void *pb)
{
    const struct file_entry *a = pa;
    const struct file_entry *b = pb;
    int result = 0;

    if (cur_opts->sort_size) {
        /* largest first */
        if (a->st.st_size != b->st.st_size)
            result = (a->st.st_size < b->st.st_size) ? 1 : -1;
    } else if (cur_opts->sort_time) {
        /* newest first */
        time_t ta = entry_time(a);
        time_t tb = entry_time(b);

        if (ta != tb)
            result = (ta < tb) ? 1 : -1;
    }
    if (result == 0)
        result = cmp_name(a, b);

    return cur_opts->reverse ? -result : result;
}

void sort_entries(struct entry_list *list, const struct options *opts)
{
    if (opts->unsorted || list->count < 2)
        return;

    cur_opts = opts;
    qsort(list->items, list->count, sizeof(*list->items), compare);
}
