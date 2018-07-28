#include "common.h"
#include "console.h"
#include "descriptor_tables.h"

extern void idt_flush(uint32_t);

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void init_idt();

idt_entry_t idt_entries[256];
dt_ptr_t idt_ptr;

void int_handler() {
    uint8_t hw[] = "Interrupt !!!";
    k_print(hw, 0x1D);
    while (1)
        ;
}

void init_idt() {
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    for (int i = 0; i < 256; i++) {
        idt_set_gate(i, (uint32_t)int_handler, 0x08, 0x8E);
    }

    idt_flush((uint32_t)&idt_ptr);
}

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}