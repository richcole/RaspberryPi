#ifndef CPU_H
#define CPU_H

#include "types.h"

extern void dummy ( uint32 );
extern uint32 spsr();
extern uint32 cpsr();
extern uint32 c1();
void wait_cycles(uint32 cycles);

uint32 irq_disabled();
uint32 fiq_disabled();
uint32 mode();
char *mode_name(uint32 mode);


#endif
