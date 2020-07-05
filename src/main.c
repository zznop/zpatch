#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include "file.h"
#include "config.h"
#include "assemble.h"

static void print_help(void)
{
    printf(
        "Zpatch v1.0 ( https://zznop.com )\n"
        "Usage: zpatch [options] input-file output-file offset\n"
        "  -h                Print usage\n"
        "  -a [arch]         Instruction set architecture of target binary. Valid inputs include:\n"
        "                      m68k      Motorola 68000 (default)\n"
        "                      x86_64    Intel x86-64\n"
    );
}

static bool is_arch_valid(char *arch)
{
    char *archs[] = { "m68k", "x86_64" };
    size_t i;

    for (i = 0; i < sizeof(archs)/sizeof(char *); i++) {
        if (!strcmp(arch, archs[i]))
            return true;
    }

    return false;
}

static bool patch_file(config_t *config)
{
    bool rv;
    mapped_file_t *inprog = NULL;

    /* Map the input file */
    rv = map_file(&inprog, config->inprog_name);
    if (rv == false)
        return false;

    rv = write_patch_assembly();
    if (rv == false)
        goto done;

done:
    unmap_file(inprog);
    return rv;
}

int main(int argc, char **argv)
{
    int opt;
    int i = 1;
    int j = 0;
    config_t config = {0};

    config.arch.archname = "m68k";
    config.offset = -1;
    while (i < argc) {
        if ((opt = getopt(argc, argv, "ha:")) != -1) {
            switch (opt) {
            case 'h':
                print_help();
                return 0;
            case 'a':
                config.arch.archname = optarg; i++;
                break;
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
        fprintf(stderr, "You must specify an input program binary\n");
        return 1;
    }

    if (!config.outprog_name) {
        fprintf(stderr, "You must specify a file path to the output program binary\n");
        return 1;
    }

    if (config.offset == (unsigned)-1) {
        fprintf(stderr, "You must specify a file offset for base of patch\n");
        return 1;
    }

    if (is_arch_valid(config.arch.archname) == false) {
        fprintf(stderr, "Invalid architecture specified: %s\n", config.arch.archname);
        return 1;
    }

    return patch_file(&config) == false;
}
