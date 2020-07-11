#ifndef _DISASSEMBLE_H
#define _DISASSEMBLE_H

#include <stdbool.h>

bool disassemble_chunk(uint8_t *buffer, size_t size, uint32_t offset);

#endif
