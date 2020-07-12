#ifndef _CONFIG_H
#define _CONFIG_H

#include <stdint.h>

typedef struct {
    char *inprog_name;
    char *outprog_name;
    uint32_t offset;
} config_t;

#define INFO "\033[0;36m[*]\033[0m "
#define SUCCESS "\033[0;32m[+]\033[0m "
#define ERR  "\033[0;31m[!]\033[0m "

#endif
