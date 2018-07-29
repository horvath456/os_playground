#include "gdt.h"
#include "idt.h"
#include "monitor.h"
#include "timer.h"
#include "types.h"

void init(void) {
    k_clrscr();
    k_print("Hello World! ", 0x07);

    init_gdt();
    init_idt();

    asm volatile("sti");

    init_timer(1000);

    int a = 1 / 0;
}
