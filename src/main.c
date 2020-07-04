#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>

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

int main(int argc, char **argv)
{
    int opt;
    int i = 0;
    int j = 0;
    char *inprog_name = NULL;
    char *outprog_name = NULL;
    char *arch = "m68k";
    uint32_t offset = -1;

    while (i < argc) {
        if ((opt = getopt(argc, argv, "ha:")) != -1) {
            switch (opt) {
            case 'h':
                print_help();
                return 0;
            case 'a':
                arch = optarg; i++;
                break;
            default:
                print_help();
                return 1;
            }
        } else {
            /* Required positional arguments (backwards) */
            switch (j) {
            case 0:
                inprog_name = argv[i];
                break;
            case 1:
                outprog_name = argv[i];
                break;
            case 2:
                offset = strtoul(argv[i], NULL, 0);
                break;
            default:
                print_help();
                return 1;
            }
            j++;
        }
        i++;
    }

    if (!inprog_name) {
        fprintf(stderr, "You must specify an input program binary\n");
        return 1;
    }

    if (!outprog_name) {
        fprintf(stderr, "You must specify a file path to the output program binary\n");
        return 1;
    }

    if (offset == (unsigned)-1) {
        fprintf(stderr, "You must specify a file offset for base of patch\n");
        return 1;
    }

    if (is_arch_valid(arch) == false) {
        fprintf(stderr, "Invalid architecture specified: %s\n", arch);
        return 1;
    }

    printf("%s %s\n", inprog_name, outprog_name);
    return 0;
}
