#include "gdt.h"
#include "isr.h"
#include "monitor.h"
#include "ports.h"
#include "tasks.h"
#include "types.h"

// todo move callback to tasks.c

registers_t* timer_callback(registers_t* regs) {
    registers_t* new_regs = schedule(regs);
    change_TSS_esp0((uint32_t)new_regs + 1);
    return new_regs;
}

void init_timer(uint32_t frequency) {
    register_irq0_handler(&timer_callback);

    uint32_t divisor = 1193180 / frequency;

    outb(0x43, 0x36);

    uint8_t l = (uint8_t)(divisor & 0xFF);
    uint8_t h = (uint8_t)((divisor >> 8) & 0xFF);

    outb(0x40, l);
    outb(0x40, h);
}