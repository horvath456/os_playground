#include "gdt.h"
#include "idt.h"
#include "keyboard.h"
#include "monitor.h"
#include "timer.h"
#include "types.h"

void init(void) {
    k_clrscr();
    kprintf("Hello World");

    init_gdt();
    init_idt();

    init_keyboard();
}
