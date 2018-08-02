#ifndef GDT_H
#define GDT_H

#include "types.h"

void init_gdt();
void change_TSS_esp0(uint32_t esp0);

#endif