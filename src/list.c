#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "utils.h"

/* Build "dir/name" (avoid a double slash if dir already ends in '/'). */
static char *join_path(const char *dir, const char *name)
{
    size_t len = strlen(dir);
    char *path = xmalloc(len + strlen(name) + 2);

    if (len > 0 && dir[len - 1] == '/')
        sprintf(path, "%s%s", dir, name);
    else
        sprintf(path, "%s/%s", dir, name);
    return path;
}

/* Decide whether a directory entry name should be listed. */
static int is_visible(const char *name, const struct options *opts)
{
    if (opts->all)
        return 1;                       /* -a: everything */
    if (name[0] != '.')
        return 1;                       /* normal file */
    if (opts->almost_all &&
        strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
        return 1;                       /* -A: hidden, but not . and .. */
    return 0;
}

int list_dir(const char *dir, const struct options *opts,
             struct entry_list *out)
{
    DIR *dp;
    struct dirent *de;
    char *path;

    dp = opendir(dir);
    if (dp == NULL) {
        fprintf(stderr, "myls: %s: %s\n", dir, strerror(errno));
        return -1;
    }

    while ((de = readdir(dp)) != NULL) {
        if (!is_visible(de->d_name, opts))
            continue;
        path = join_path(dir, de->d_name);
        list_add(out, de->d_name, path);   /* prints its own error */
        free(path);
    }
    closedir(dp);
    return 0;
}
