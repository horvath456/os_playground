#ifndef IDT_H
#define IDT_H

#include "common.h"

#define GDT_FLAG_DATASEG 0x02
#define GDT_FLAG_CODESEG 0x0a
#define GDT_FLAG_TSS 0x09
#define GDT_FLAG_SEGMENT 0x10
#define GDT_FLAG_RING0 0x00
#define GDT_FLAG_RING3 0x60
#define GDT_FLAG_PRESENT 0x80
#define GDT_FLAG_4K_GRAN 0x800
#define GDT_FLAG_32_BIT 0x400

struct gdt_entry_struct {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct idt_entry_struct {
    uint16_t base_lo;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_hi;
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct dt_ptr_struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));
typedef struct dt_ptr_struct dt_ptr_t;

#endif