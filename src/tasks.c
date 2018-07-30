#include "isr.h"
#include "monitor.h"
#include "types.h"

void task_a() {
    while (1) {
        asm volatile("cli");
        k_print("A", 0x9B);
        asm volatile("sti");
    }
}

void task_b() {
    while (1) {
        asm volatile("cli");  // critical section !!
        k_print("B", 0xA4);
        asm volatile("sti");
    }
}

static uint8_t stack_a[4096];
static uint8_t stack_b[4096];

/*

* Jeder Task braucht seinen eigenen Stack, auf dem er beliebig arbeiten kann,
* ohne dass ihm andere Tasks Dinge ueberschreiben. Ausserdem braucht ein Task
* einen Einsprungspunkt.
*/
registers_t* init_task(uint8_t* stack, void* entry) {
    /*
     * CPU-Zustand fuer den neuen Task festlegen
     */
    registers_t new_state = {
        .eax = 0,
        .ebx = 0,
        .ecx = 0,
        .edx = 0,
        .esi = 0,
        .edi = 0,
        .ebp = 0,
        //.esp = unbenutzt (kein Ring-Wechsel)
        .eip = (uint32_t)entry,
        /* Ring-0-Segmentregister */
        .cs = 0x08,
        //.ss  = unbenutzt (kein Ring-Wechsel)
        /* IRQs einschalten (IF = 1) */
        .eflags = 0x202,
    };
    /*
     * Den angelegten CPU-Zustand auf den Stack des Tasks kopieren, damit es am
     * Ende so aussieht als waere der Task durch einen Interrupt unterbrochen
     * worden. So kann man dem Interrupthandler den neuen Task unterschieben
     * und er stellt einfach den neuen Prozessorzustand "wieder her".
     */
    registers_t* state = (void*)(stack + 4096 - sizeof(new_state));
    *state = new_state;
    return state;
}

static int current_task = -1;
static int num_tasks = 2;
static registers_t* task_states[2];

void init_multitasking() {
    task_states[0] = init_task(stack_a, task_a);
    task_states[1] = init_task(stack_b, task_b);
}

/*

* Gibt den Prozessorzustand des naechsten Tasks zurueck. Der aktuelle
* Prozessorzustand wird als Parameter uebergeben und gespeichert, damit er
* beim naechsten Aufruf des Tasks wiederhergestellt werden kann
*/
registers_t* schedule(registers_t* cpu) {
    /*
     * Wenn schon ein Task laeuft, Zustand sichern. Wenn nicht, springen wir
     * gerade zum ersten Mal in einen Task. Diesen Prozessorzustand brauchen
     * wir spaeter nicht wieder.
     */
    if (current_task >= 0) {
        task_states[current_task] = cpu;
    }
    /*
     * Naechsten Task auswaehlen. Wenn alle durch sind, geht es von vorne los
     */
    current_task++;
    current_task %= num_tasks;
    /* Prozessorzustand des neuen Tasks aktivieren */
    cpu = task_states[current_task];
    return cpu;
}