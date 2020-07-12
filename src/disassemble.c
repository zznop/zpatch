#include <capstone/capstone.h>
#include "disassemble.h"
#include "config.h"

#define MAX_DISAS_SIZE 128

bool disassemble_chunk(uint8_t *buffer, size_t size, uint32_t offset)
{
    uint32_t disas_size = MAX_DISAS_SIZE;
    csh handle;
    cs_insn *insn;
    int count;
    int i;

    if (offset + MAX_DISAS_SIZE > size)
        disas_size = size - offset;

    if (cs_open(CS_ARCH_M68K, CS_MODE_BIG_ENDIAN|CS_MODE_M68K_000, &handle))
        return false;

    count = cs_disasm(handle, buffer+offset, disas_size, offset, 0, &insn);
    if (count <= 0) {
        /* TODO: print hex dump */
        return true;
    }

    printf(INFO "Patch will overwrite one or more of the following instructions:\n\n");
    for (i = 0; i < count; i++)
        printf("  0x%" PRIx64 ":  %s    %s\n", insn[i].address, insn[i].mnemonic, insn[i].op_str);
    printf("\n");

    cs_free(insn, count);
    cs_close(&handle);
    return true;
}
