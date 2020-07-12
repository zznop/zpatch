#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include "file.h"
#include "config.h"
#include "disassemble.h"
#include "assemble.h"

static void print_banner(void)
{
    printf(
        "\033[0;33m"
        " ____  ____   __   ____  ___  _   _\n"
        "(_   )(  _ \\ /__\\ (_  _)/ __)( )_( )\n"
        " / /_  )___//(__)\\  )( ( (__  ) _ (\n"
        "(____)(__) (__)(__)(__) \\___)(_) (_)\n\n"
        "\033[0m"
    );
}

static void print_help(void)
{
    printf(
        "Zpatch v1.0 ( https://zznop.com )\n"
        "Usage: zpatch [options] input-file output-file offset\n"
        "  -h                Print usage\n"
    );
}

static bool patch_file(config_t *config)
{
    bool rv;
    mapped_file_t *inprog = NULL;
    mapped_file_t *patch = NULL;

    /* Map the input file */
    rv = map_file(&inprog, config->inprog_name);
    if (rv == false)
        return false;

    /* Display instructions in region the user wants to patch */
    rv = disassemble_chunk(inprog->data, inprog->size, config->offset);
    if (rv == false)
        goto done;

    printf("Press ENTER key to continue\n");  
    getchar();

    rv = write_patch_assembly();
    if (rv == false)
        goto done;

    printf(INFO "Attempting to compile the patch file ...\n");
    rv = assemble_patch();
    if (rv == false) {
        fprintf(stderr, ERR "Failed to assemble patch file\n");
        goto done;
    }

    printf(INFO "Extracting patch code from object file ...\n");
    rv = extract_bin();
    if (rv == false) {
        fprintf(stderr, ERR "Failed to extract patch code\n");
        goto done;
    }

    printf(INFO "Loading patch file data into memory ...\n");
    rv = map_file(&patch, PATCH_BIN);
    if (rv == false)
        goto done;

    printf(INFO "Applying patch ...\n");
    rv = export_patched_file(inprog, patch, config->offset, config->outprog_name);
    if (rv == false)
        goto done;
done:
    unmap_file(inprog);
    unmap_file(patch);
    cleanup_patch_artifacts();
    return rv;
}

int main(int argc, char **argv)
{
    int opt;
    int i = 1;
    int j = 0;
    config_t config = {0};

    config.offset = -1;
    while (i < argc) {
        if ((opt = getopt(argc, argv, "h")) != -1) {
            switch (opt) {
            case 'h':
                print_help();
                return 0;
            default:
                print_help();
                return 1;
            }
        } else {
            switch (j) {
            case 0:
                config.inprog_name = argv[i];
                break;
            case 1:
                config.outprog_name = argv[i];
                break;
            case 2:
                config.offset = strtoul(argv[i], NULL, 0);
                break;
            default:
                print_help();
                return 1;
            }
            j++;
        }
        i++;
    }

    if (!config.inprog_name) {
        fprintf(stderr, ERR "You must specify an input program binary\n");
        return 1;
    }

    if (!config.outprog_name) {
        fprintf(stderr, ERR "You must specify a file path to the output program binary\n");
        return 1;
    }

    if (config.offset == (unsigned)-1) {
        fprintf(stderr, ERR "You must specify a file offset for base of patch\n");
        return 1;
    }

    print_banner();
    return patch_file(&config) == false;
}
