#ifndef IRQ_H
#define IRQ_H

#include "types.h"

extern void enable_irq();
extern void disable_irq();

extern uint32 volatile *irq_enable;
extern uint32 volatile *irq_disable;

#endif

