#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "file.h"

static int _get_file_size(const char *filepath)
{
    struct stat st;
    if (stat(filepath, &st) < 0)
        return -1;
    return st.st_size;
}

bool map_file(mapped_file_t **file, const char *filename)
{
    int size;
    int fd;
    void *data = NULL;

    *file = NULL;
    size = _get_file_size(filename);
    if (size < 0) {
        fprintf(stderr, "Failed to get file size: %s\n", filename);
        return false;
    }

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "Failed to open file\n");
        return false;
    }

    data = (void *)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (!data) {
        fprintf(stderr, "Failed to map file\n");
        goto error;
    }

    *file = (mapped_file_t *)malloc(sizeof(mapped_file_t));
    if (!*file) {
        fprintf(stderr, "Failed to allocate memory for file mapping");
        goto error;
    }

    (*file)->fd = fd;
    (*file)->size = size;
    (*file)->data = data;
    return true;
error:
    free(*file);
    if (data)
        munmap(data, size);
    if (fd != -1)
        close(fd);
    return false;
}

void unmap_file(mapped_file_t *file)
{
    if (!file)
        return;

    if (file->data)
        munmap(file->data, file->size);
    if (file->fd != -1)
        close(file->fd);
    free(file);
}

