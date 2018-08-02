#include "descriptor_tables.h"
#include "types.h"

#define GDT_ENTRIES 6

extern void gdt_flush(uint32_t);

void init_gdt();
void change_TSS_esp0(uint32_t esp0);
static void gdt_set_gate(sint32_t, uint32_t, uint32_t, uint8_t, uint8_t);

gdt_entry_t gdt_entries[GDT_ENTRIES];
dt_ptr_t gdt_ptr;

uint32_t tss[32] = {0x0, 0x0, 0x10};

void init_gdt() {
    gdt_ptr.limit = (sizeof(gdt_entry_t) * GDT_ENTRIES) - 1;
    gdt_ptr.base = (uint32_t)&gdt_entries;

    gdt_set_gate(0, 0, 0, 0, 0);                 // Null segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // Code segment
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // Data segment
    gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);  // User mode code segment
    gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);  // User mode data segment
    gdt_set_gate(5, (uint32_t)tss, (uint32_t)tss + sizeof(tss), 0xE9,
                 0xCF);  // TSS

    gdt_flush((uint32_t)&gdt_ptr);

    // Taskregister neu laden
    asm volatile("ltr %%ax" : : "a"(5 << 3));
}

void change_TSS_esp0(uint32_t esp0) { tss[1] = esp0; }

// Set the value of one GDT entry.
static void gdt_set_gate(sint32_t num, uint32_t base, uint32_t limit,
                         uint8_t access, uint8_t gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}