#include "isr.h"
#include "monitor.h"
#include "ports.h"
#include "types.h"

static void send_command(uint8_t command);
static registers_t* irq_handler(registers_t* regs);
void init_keyboard();
void handle_keycode(uint8_t keycode);
uint8_t translate_scancode(int set, uint16_t scancode);

static uint8_t sc_to_kc[][128] = {
    // Normale Scancodes
    {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18,
     19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
     38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
     57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,
     76, 77, 78, 79, 80, 81, 82, 84, 00, 00, 86, 87, 88, 00, 00, 00, 00, 00, 00,
     00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00,
     00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},

    // Extended0-Scancodes (werden mit e0 eingeleitet)
    {00,  00,  00,  00,  00, 00, 00, 00,  00,  00,  00, 00,  00, 00,  00, 00,
     00,  00,  00,  00,  00, 00, 00, 00,  00,  00,  00, 00,  96, 97,  00, 00,
     00,  00,  00,  00,  00, 00, 00, 00,  00,  00,  00, 00,  00, 00,  00, 00,
     00,  00,  00,  00,  00, 99, 00, 00,  100, 00,  00, 00,  00, 00,  00, 00,
     00,  00,  00,  00,  00, 00, 00, 102, 103, 104, 00, 105, 00, 106, 00, 107,
     108, 109, 110, 111, 00, 00, 00, 00,  00,  00,  00, 00,  00, 00,  00, 00,
     00,  00,  00,  00,  00, 00, 00, 00,  00,  00,  00, 00,  00, 00,  00, 00,
     00,  00,  00,  00,  00, 00, 00, 00,  00,  00,  00, 00,  00, 00,  00, 00},
};

void init_keyboard() {
    register_interrupt_handler(IRQ1, &irq_handler);
    // Tastaturpuffer leeren
    while (inb(0x64) & 0x1) {
        inb(0x60);
    }
    // Tastatur aktivieren
    send_command(0xF4);
}

static void send_command(uint8_t command) {
    // Warten bis die Tastatur bereit ist, und der Befehlspuffer leer ist
    while ((inb(0x64) & 0x2)) {
    }
    outb(0x60, command);
}

static registers_t* irq_handler(registers_t* regs) {
    uint8_t scancode;
    uint8_t keycode = 0;
    int break_code = 0;

    // Status-Variablen fuer das Behandeln von e0- und e1-Scancodes
    static int e0_code = 0;
    // Wird auf 1 gesetzt, sobald e1 gelesen wurde, und auf 2, sobald das erste
    // Datenbyte gelesen wurde
    static int e1_code = 0;
    static uint16_t e1_prev = 0;

    scancode = inb(0x60);

    // Um einen Breakcode handelt es sich, wenn das oberste Bit gesetzt ist und
    // es kein e0 oder e1 fuer einen Extended-scancode ist
    if ((scancode & 0x80) && (e1_code || (scancode != 0xE1)) &&
        (e0_code || (scancode != 0xE0))) {
        break_code = 1;
        scancode &= ~0x80;
    }

    if (e0_code) {
        // Fake shift abfangen und ignorieren
        if ((scancode == 0x2A) || (scancode == 0x36)) {
            e0_code = 0;
            return regs;
        }

        keycode = translate_scancode(1, scancode);
        e0_code = 0;
    } else if (e1_code == 2) {
        // Fertiger e1-Scancode
        // Zweiten Scancode in hoeherwertiges Byte packen
        e1_prev |= ((uint16_t)scancode << 8);
        keycode = translate_scancode(2, e1_prev);
        e1_code = 0;
    } else if (e1_code == 1) {
        // Erstes Byte fuer e1-Scancode
        e1_prev = scancode;
        e1_code++;
    } else if (scancode == 0xE0) {
        // Anfang eines e0-Codes
        e0_code = 1;
    } else if (scancode == 0xE1) {
        // Anfang eines e1-Codes
        e1_code = 1;
    } else {
        // Normaler Scancode
        keycode = translate_scancode(0, scancode);
    }

    kprintf("keycode: %d", keycode);

    return regs;
}

uint8_t translate_scancode(int set, uint16_t scancode) {
    uint8_t keycode = 0;

    switch (set) {
        case 0:
            keycode = sc_to_kc[0][scancode];
            break;
        case 1:
            keycode = sc_to_kc[1][scancode];
            break;
        case 2:
            switch (scancode) {
                case 0x451D:
                    keycode = 119;
                    break;
                default:
                    keycode = 0x0;
            };
            break;
    }

    return keycode;
}
