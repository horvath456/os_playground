#include "gdt.h"
#include "idt.h"
#include "monitor.h"
#include "timer.h"
#include "types.h"

void init(void) {
    k_clrscr();
    kprintf("Hello %d %x Worl %s d!", 5, 0xAFFE, "substr");

    init_gdt();
    init_idt();

    asm volatile("sti");

    init_timer(1000);
}
