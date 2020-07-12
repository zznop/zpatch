#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "file.h"
#include "config.h"

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
        fprintf(stderr, ERR "Failed to get file size: %s\n", filename);
        return false;
    }

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, ERR "Failed to open file\n");
        return false;
    }

    data = (void *)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (!data) {
        fprintf(stderr, ERR "Failed to map file\n");
        goto error;
    }

    *file = (mapped_file_t *)malloc(sizeof(mapped_file_t));
    if (!*file) {
        fprintf(stderr, ERR "Failed to allocate memory for file mapping");
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

bool export_patched_file(mapped_file_t *inprog, mapped_file_t *patch, uint32_t offset, char *outfile)
{
    int fd;
    int n;
    int remaining;
    bool rv = false;

    fd = open(outfile, O_RDWR|O_CREAT|O_TRUNC, 0777);
    if (fd == -1) {
        fprintf(stderr, ERR "Failed to create patched file\n");
        return false;
    }

    printf(INFO "Writing data until patch offset (size: %u) ...\n", offset);
    n = write(fd, inprog->data, offset);
    if ((uint32_t)n != offset) {
        fprintf(stderr, ERR "Failed to export patched file (initial data write)\n");
        goto done;
    }

    printf(INFO "Writing patch file (size: %u) ...\n", patch->size);
    n = write(fd, patch->data, patch->size);
    if (n != patch->size) {
        fprintf(stderr, ERR "Failed to export patched file (patch write)\n");
        goto done;
    }

    remaining = inprog->size - offset - patch->size;
    printf(INFO "Writing remaining data (size: %u) ...\n",  remaining);
    n = write(fd, inprog->data+offset+patch->size, remaining);
    if (n != remaining) {
        fprintf(stderr, ERR "Failed to export patched file (remaining data write)\n");
        goto done;
    }

    printf(SUCCESS "Patch applied successfully! (File path: %s)\n", outfile);
    rv = true;
done:
    close(fd);
    return rv;
}
