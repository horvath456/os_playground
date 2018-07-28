#include "common.h"
#include "console.h"
#include "gdt.h"
#include "idt.h"

void init(void) {
    uint8_t hw[] = "Hello World! ";

    k_clrscr();
    k_print(hw, 0x07);
    k_printhex(0x65AF, 0x07);

    init_gdt();
    init_idt();

    int a = 4 / 0;
}
