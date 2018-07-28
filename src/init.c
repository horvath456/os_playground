#include "gdt.h"
#include "idt.h"
#include "monitor.h"
#include "types.h"

void init(void) {
    k_clrscr();
    k_print("Hello World! ", 0x07);
    k_printhex(0x65AF, 0x07);

    init_gdt();
    init_idt();

    asm volatile("int $0x3");
}
