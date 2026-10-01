#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "entry.h"
#include "list.h"
#include "options.h"
#include "print.h"
#include "sort.h"
#include "utils.h"

/*
 * List one directory. If -R is set, recurse into subdirectories.
 * print_heading: print "name:" first (needed with several operands or -R).
 * Returns 0 on success, 1 on any error.
 */
static int show_dir(const char *name, const struct options *opts,
                    int print_heading)
{
    struct entry_list contents;
    int status = 0;
    size_t k;

    if (print_heading)
        printf("%s:\n", name);

    list_init(&contents);
    if (list_dir(name, opts, &contents) == -1) {
        list_free(&contents);
        return 1;
    }
    sort_entries(&contents, opts);
    print_entries(&contents, opts, 1);

    if (opts->recursive) {
        for (k = 0; k < contents.count; k++) {
            const struct file_entry *e = &contents.items[k];

            /* Real directories only (lstat: symlinks are not followed),
             * and never "." or ".." (avoids infinite recursion). */
            if (!S_ISDIR(e->st.st_mode))
                continue;
            if (strcmp(e->name, ".") == 0 || strcmp(e->name, "..") == 0)
                continue;
            printf("\n");
            if (show_dir(e->path, opts, 1) != 0)
                status = 1;
        }
    }
    list_free(&contents);
    return status;
}

/*
 * Handle the operands given on the command line.
 * Returns the exit status: 0 if everything worked, 1 on any error.
 */
static int process_operands(int count, char *names[],
                            const struct options *opts)
{
    struct entry_list files, dirs;
    int status = 0;
    int i;
    size_t k;
    int printed_something = 0;

    list_init(&files);
    list_init(&dirs);

    /* Split operands into non-directories and directories. */
    for (i = 0; i < count; i++) {
        struct entry_list *target;
        struct stat st;

        if (lstat(names[i], &st) == -1) {
            /* list_add prints the error; just record the failure. */
            list_add(&files, names[i], names[i]);
            status = 1;
            continue;
        }
        target = S_ISDIR(st.st_mode) && !opts->dir_as_file ? &dirs : &files;
        list_add(target, names[i], names[i]);
    }

    sort_entries(&files, opts);
    sort_entries(&dirs, opts);

    /* Non-directory operands first. */
    if (files.count > 0) {
        print_entries(&files, opts, 0);
        printed_something = 1;
    }

    /* Then each directory. */
    for (k = 0; k < dirs.count; k++) {
        if (printed_something)
            printf("\n");
        if (show_dir(dirs.items[k].name, opts,
                     count > 1 || opts->recursive) != 0)
            status = 1;
        printed_something = 1;
    }

    list_free(&files);
    list_free(&dirs);
    return status;
}

int main(int argc, char *argv[])
{
    struct options opts;
    int first = parse_options(argc, argv, &opts);

    if (first >= argc) {
        char *cwd[1] = { "." };
        return process_operands(1, cwd, &opts);
    }
    return process_operands(argc - first, argv + first, &opts);
}
