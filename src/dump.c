#include "isr.h"
#include "monitor.h"
#include "types.h"

void cpu_dump(registers_t* cpu) {
    kprintf("CPU DUMP:");
    kprintf("EIP: %x   CS: %x   EFLAGS: %x   USERESP: %x   SS: %x", cpu->eip,
            cpu->cs, cpu->eflags, cpu->useresp, cpu->ss);
    kprintf("DS: %x   EDI: %x   ESI: %x   EBP: %x   ESP: %x", cpu->ds, cpu->edi,
            cpu->esi, cpu->ebp, cpu->esp);
    kprintf("EAX: %x   EBX: %x   ECX: %x   EDX: %x\n", cpu->eax, cpu->ebx,
            cpu->ecx, cpu->edx);
}