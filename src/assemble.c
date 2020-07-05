#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assemble.h"

static bool _touch_source_file(void)
{
    bool rv = false;
    FILE *file;
    int header_len;
    int n;
    const char *header = (
        ".section .text\n"
        ".global _start\n"
        "\n"
        "_start:\n"
    );

    file = fopen(SOURCE_FILENAME, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to create patch source file\n");
        return false;
    }

    header_len = strlen(header);
    n = fwrite(header, 1, header_len, file);
    if (n != header_len) {
        fprintf(stderr, "Failed to write to patch source file\n");
        goto out;
    }
    rv = true;
out:
    fclose(file);
    return rv;
}

bool write_patch_assembly(void)
{
    if (_touch_source_file() == false)
        return false;

    return true;
}
