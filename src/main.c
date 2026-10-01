#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "entry.h"
#include "list.h"
#include "options.h"
#include "print.h"
#include "utils.h"

/*
 * Handle the operands given on the command line.
 * Returns the exit status: 0 if everything worked, 1 on any error.
 */
static int process_operands(int count, char *names[],
                            const struct options *opts)
{
    struct entry_list files, dirs, contents;
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
        /* Without -d a symlink to a directory would be followed by real ls
         * only with -H/-L; here we treat a real directory only. */
        target = S_ISDIR(st.st_mode) && !opts->dir_as_file ? &dirs : &files;
        list_add(target, names[i], names[i]);
    }

    /* Non-directory operands first. */
    if (files.count > 0) {
        print_entries(&files, opts);
        printed_something = 1;
    }

    /* Then each directory. */
    for (k = 0; k < dirs.count; k++) {
        const char *name = dirs.items[k].name;

        if (printed_something)
            printf("\n");
        if (count > 1)
            printf("%s:\n", name);

        list_init(&contents);
        if (list_dir(name, opts, &contents) == -1)
            status = 1;
        else
            print_entries(&contents, opts);
        list_free(&contents);
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
