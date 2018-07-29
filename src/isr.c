#include "isr.h"
#include "monitor.h"
#include "panic.h"
#include "ports.h"
#include "types.h"

registers_t irq_handler(registers_t regs);
void register_interrupt_handler(uint8_t n, isr_t handler);
void isr_handler(registers_t regs);

isr_t interrupt_handlers[256];

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void isr_handler(registers_t regs) {
    if (regs.int_no <= 0x1f) {
        kernel_panic(regs.int_no, regs.err_code);
    } else {
        return irq_handler(regs);
    }
}

registers_t irq_handler(registers_t regs) {
    if (regs.int_no >= 0x28) {
        outb(0xa0, 0x20);
    }
    outb(0x20, 0x20);

    registers_t new_cpu = regs;

    if (interrupt_handlers[regs.int_no] != 0) {
        isr_t handler = interrupt_handlers[regs.int_no];
        new_cpu = *handler(regs);
    }

    return new_cpu;
}