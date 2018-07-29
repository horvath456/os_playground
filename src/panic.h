#ifndef PANIC_H
#define PANIC_H

#include "types.h"

void kernel_panic(uint32_t int_no, uint32_t error_code);

#endif