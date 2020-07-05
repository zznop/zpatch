#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>

struct architecture {
    char *archname;
    int cs_arch;
};

typedef struct {
    char *inprog_name;
    char *outprog_name;
    uint32_t offset;
    struct architecture arch;
} config_t;

#endif
