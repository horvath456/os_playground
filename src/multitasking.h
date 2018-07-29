#ifndef MULTITASKING_H
#define MULTITASKING_H

#include "isr.h"
#include "types.h"

registers_t* schedule(registers_t* cpu);

#endif