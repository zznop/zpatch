#ifndef _CONFIG_H
#define _CONFIG_H

struct architecture {
    char *archname;
    int cs_arch;
};

typedef struct {
    char *inprog_name;
    char *outprog_name;
    uint32_t offset;
    struct architecture;
} config_t;

#endif
