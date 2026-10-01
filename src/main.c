#include "ls.h"
#include "options.h"

int main(int argc, char *argv[])
{
    struct options opts;
    int first = parse_options(argc, argv, &opts);

    /* No operands: list the current directory. */
    if (first >= argc) {
        char *cwd[1] = { "." };
        return ls_run(1, cwd, &opts);
    }
    return ls_run(argc - first, argv + first, &opts);
}
