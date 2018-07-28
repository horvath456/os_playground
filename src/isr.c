#include "isr.h"
#include "monitor.h"
#include "types.h"

void isr_handler(registers_t regs) {
    k_print("interrupt handler", 0x69);
    k_printdec(regs.int_no, 0x07);
    k_print(" hihi\n", 0x07);
    while (1)
        ;
}