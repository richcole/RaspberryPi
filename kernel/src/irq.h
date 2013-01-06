#ifndef IRQ_H
#define IRQ_H

#include "types.h"

void enable_irq();
void disable_irq();
extern struct irq_t volatile *irq;
void enable_intr(uint32 intr);
void disable_intr(uint32 intr);

#endif

