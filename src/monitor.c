#include "monitor.h"
#include <stdarg.h>
#include "ports.h"
#include "types.h"

#define ROWS 25
#define COLS 80

uint8_t* video = (uint8_t*)0xb8000;
uint16_t* video_word = (uint16_t*)0xb8000;

uint8_t cursor_x = 0;
uint8_t cursor_y = 0;

static void move_cursor() {
    uint16_t cursorLocation = cursor_y * COLS + cursor_x;
    outb(0x3D4, 14);  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8);  // Send the high cursor byte.
    outb(0x3D4, 15);  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);  // Send the low cursor byte.
}

static void scroll() {
    uint16_t blank = 0x00;

    if (cursor_y >= ROWS) {
        for (int i = 0; i < (ROWS - 1) * COLS; i++) {
            video_word[i] = video_word[i + COLS];
        }

        for (int i = (ROWS - 1) * COLS; i < ROWS * COLS; i++) {
            video_word[i] = blank;
        }
        cursor_y = (ROWS - 1);
    }
}

void k_clrscr() {
    for (int i = 0; i < ROWS * COLS * 2; i++) {
        video[i] = 0x0;
    }

    cursor_x = 0;
    cursor_y = 0;
}

void k_putc(uint8_t c, uint8_t attr) {
    if (c == '\n') {
        cursor_y++;
        cursor_x = 0;
    } else {
        video[cursor_y * COLS * 2 + cursor_x * 2] = c;
        video[cursor_y * COLS * 2 + cursor_x * 2 + 1] = attr;

        cursor_x++;
    }

    if (cursor_x > 80) {
        cursor_x = 0;
        cursor_y++;
    }

    scroll();
    move_cursor();
}

void k_print(const char* str, uint8_t attr) {
    for (int i = 0; str[i] != '\0'; i++) {
        k_putc((uint8_t)str[i], attr);
    }
}

void k_printdec(uint32_t a, uint8_t attr) {
    char buf[12];
    buf[11] = 0x0;

    int i = 11;

    while (1) {
        uint8_t digit = a % 10;
        buf[--i] = digit + 48;
        a /= 10;
        if (a == 0) break;
    }

    k_print(buf + i, attr);
}

void k_printhex(uint32_t a, uint8_t attr) {
    char buf[9] = "00000000\0";

    int i = 8;

    while (a >= 1) {
        uint8_t digit = a % 16;
        buf[--i] = digit < 10 ? digit + 48 : digit + 55;
        a /= 16;
    }

    k_print("0x", attr);
    k_print(buf, attr);
}

void kprintf(const char* format, ...) {
    va_list parameters;
    va_start(parameters, format);

    uint8_t attr = 0x07;

    for (int i = 0; format[i] != '\0'; i++) {
        char c = format[i];

        if (c == '%') {
            if (format[i + 1] == 'd') {
                uint32_t d = (uint32_t)va_arg(parameters, int);
                k_printdec(d, attr);
            } else if (format[i + 1] == 'x') {
                uint32_t d = (uint32_t)va_arg(parameters, int);
                k_printhex(d, attr);
            } else if (format[i + 1] == 's') {
                const char* str = va_arg(parameters, const char*);
                k_print(str, attr);
            }
            i++;
            continue;
        } else {
            k_putc(c, attr);
        }
    }

    k_print("\n", attr);

    va_end(parameters);
}