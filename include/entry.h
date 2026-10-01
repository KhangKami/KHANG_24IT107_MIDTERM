#ifndef ENTRY_H
#define ENTRY_H

#include <sys/stat.h>
#include <stddef.h>

/* One file to be displayed: its name, full path, and lstat() data. */
struct file_entry {
    char *name;         /* name shown to the user             */
    char *path;         /* path used for lstat()/readlink()   */
    struct stat st;     /* result of lstat(path)              */
};

/* A growable array of file entries. */
struct entry_list {
    struct file_entry *items;
    size_t count;       /* number of entries in use */
    size_t capacity;    /* allocated slots          */
};

/* Initialise an empty list. */
void list_init(struct entry_list *list);

/*
 * Append an entry. name is what is displayed, path is what is passed
 * to lstat(). Returns 0 on success, -1 if lstat() fails (an error
 * message is printed and the entry is not added).
 */
int list_add(struct entry_list *list, const char *name, const char *path);

/* Free all memory held by the list. */
void list_free(struct entry_list *list);

#endif
