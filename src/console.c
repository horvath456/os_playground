#include "common.h"
#include "console.h"

uint8* video = (uint8*) 0xb8000;

uint8 x_screen = 0;
uint8 y_screen = 0;

void k_clrscr() {
    for(int i=0; i<ROWS*COLS*2; i++) {
        video[i] = 0x0;
    }
}

void k_putc(uint8 c, uint8 attr) {
    video[y_screen*COLS*2+x_screen*2] = c;
    video[y_screen*COLS*2+x_screen*2+1] = attr;

    x_screen++;
    if(x_screen > 80) {
        x_screen = 0;
        y_screen++;
    }
}

void k_print(uint8 *str, uint8 attr) {
   for (int i = 0;str[i] != '\0'; i++) {
       k_putc(str[i], attr);
   }
}

void k_printdec(uint32 a, uint8 attr) {
    uint8 buf[12];
    buf[11] = 0x0;

    int i = 11;

    while(a >= 1) {
        uint8 digit = a % 10;
        buf[--i] = digit+48;
        a /= 10;
    }

    k_print(buf+i, attr);
}

void k_printhex(uint32 a, uint8 attr) {
    uint8 prefix[] = "0x";

    uint8 buf[9];
    buf[8] = 0x0;

    int i = 8;

    while(a >= 1) {
        uint8 digit = a % 16;
        buf[--i] = digit < 10 ? digit+48 : digit+55;
        a /= 16;
    }

    k_print(prefix, attr);
    k_print(buf+i, attr);
}