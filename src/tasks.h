#ifndef TASKS_H
#define TASKS_H

#include "isr.h"

registers_t* schedule(registers_t* cpu);
void init_multitasking();

#endif