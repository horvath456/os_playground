#include "drivers/kbc/keyboard.h"
#include "gdt.h"
#include "idt.h"
#include "monitor.h"
#include "tasks.h"
#include "timer.h"
#include "types.h"

void init(void) {
    k_clrscr();
    kprintf("Hello World");

    init_gdt();
    init_idt();

    init_keyboard();

    init_multitasking();
    init_timer(10);
}
