#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int fd;
    int size;
    uint8_t *data;
} mapped_file_t;

bool map_file(mapped_file_t **file, const char *filename);
void unmap_file(mapped_file_t *file);
bool export_patched_file(mapped_file_t *inprog, mapped_file_t *patch, uint32_t offset, char *outfile);
