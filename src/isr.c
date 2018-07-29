#include "isr.h"
#include "monitor.h"
#include "ports.h"
#include "types.h"

void isr_handler(registers_t regs) {
    k_print("interrupt handler ", 0x07);
    k_printdec(regs.int_no, 0x07);
    k_print(" hihi", 0x07);

    if (regs.int_no >= 0x20 && regs.int_no <= 0x2f) {
        if (regs.int_no >= 0x28) {
            outb(0xa0, 0x20);
        }
        outb(0x20, 0x20);
    }
}