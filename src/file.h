#ifndef _FILE_H
#define _FILE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    int fd;
    int size;
    uint8_t *data;
} mapped_file_t;

/**
 * Map in a file and return a populated file mapping structure
 *
 * @param file Uninitialized pointer to output file mapping structure
 * @param filename Path to file
 * @return true for success, false for failure
 */
bool map_file(mapped_file_t **file, const char *filename);

/**
 * Unload file mapping and cleanup memory
 *
 * @param file File mapping structure
 */
void unmap_file(mapped_file_t *file);

/**
 * Apply patch and export file
 *
 * @param inprog Mapped input file
 * @param patch Mapped patch file
 * @param offset Base offset for patch application
 * @param outfile File patch to patched file
 * @return true for success, false for failure
 */
bool export_patched_file(mapped_file_t *inprog, mapped_file_t *patch, uint32_t offset, char *outfile);

#endif
