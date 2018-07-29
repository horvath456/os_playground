#include "monitor.h"
#include "types.h"

void kernel_panic(uint32_t int_no, uint32_t error_code) {
    k_print("KERNEL PANIC!!", 0xA5);

    while (1) {
        // Prozessor anhalten
        asm volatile("cli; hlt");
    }
}