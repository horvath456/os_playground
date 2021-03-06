#include "isr.h"
#include "monitor.h"
#include "panic.h"
#include "ports.h"
#include "types.h"

void register_interrupt_handler(uint8_t n, isr_t handler);
registers_t* isr_handler(registers_t* regs);
static registers_t* handle_irq(registers_t* regs);

isr_t interrupt_handlers[256];
isr_irq0_t irq0_handler;

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void register_irq0_handler(isr_irq0_t handler) { irq0_handler = handler; }

registers_t* isr_handler(registers_t* regs) {
    registers_t* new_regs = regs;

    if (regs->int_no <= 0x1f) {
        kernel_panic(regs->int_no, regs->err_code);
    } else {
        new_regs = handle_irq(regs);
    }

    return new_regs;
}

static registers_t* handle_irq(registers_t* regs) {
    registers_t* new_regs = regs;

    if (regs->int_no >= 0x28) {
        outb(0xa0, 0x20);
    }
    outb(0x20, 0x20);

    if (regs->int_no == IRQ0 && irq0_handler != 0) {
        new_regs = irq0_handler(regs);
    } else if (interrupt_handlers[regs->int_no] != 0) {
        isr_t handler = interrupt_handlers[regs->int_no];
        handler();
    }

    return new_regs;
}