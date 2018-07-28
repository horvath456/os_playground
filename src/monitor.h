#ifndef MONITOR_H
#define MONITOR_H

#include "types.h"

void k_clrscr();
void k_putc(uint8_t c, uint8_t attr);
void k_print(uint8_t* str, uint8_t attr);
void k_printdec(uint32_t a, uint8_t attr);
void k_printhex(uint32_t a, uint8_t attr);

#endif