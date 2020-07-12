#ifndef _ASSEMBLE_H
#define _ASSEMBLE_H

#include <stdbool.h>

#define AS           "/usr/bin/m68k-linux-gnu-as"
#define OBJCOPY      "/usr/bin/m68k-linux-gnu-objcopy"
#define PATCH_SOURCE "patch.S"
#define PATCH_OBJECT "patch.o"
#define PATCH_BIN    "patch.bin"

bool write_patch_assembly(void);
bool assemble_patch(void);
bool extract_bin();
void cleanup_patch_artifacts(void);

#endif
