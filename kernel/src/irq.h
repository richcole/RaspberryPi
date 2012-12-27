#ifndef IRQ_H
#define IRQ_H

#include "types.h"

struct irq_t {
  uint32 pending_basic;
  uint32 pending[2];
  uint32 fiq_control;
  uint32 enable[2];
  uint32 enable_basic;
  uint32 disable[2];
  uint32 disable_basic;
} irq_t;

void enable_irq();
void disable_irq();
extern struct irq_t volatile *irq;

#endif

