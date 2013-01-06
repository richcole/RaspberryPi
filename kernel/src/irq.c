#include "irq.h"
#include "uart.h"
#include "timer.h"

struct irq_t {
  uint32 pending_basic;
  uint32 pending[2];
  uint32 fiq_control;
  uint32 enable[2];
  uint32 enable_basic;
  uint32 disable[2];
  uint32 disable_basic;
} irq_t;

extern struct irq_t volatile *irq;

uint32 irq_pending(uint32 intr) {
  return irq->pending[0] & intr;
}

uint32 c_irq_handler() {
  if ( irq_pending(uart_intr) ) {
    print_buf("uart intr:\n");
    uart_echo();
  }

  if ( irq_pending(timer_intr) ) {
    print_buf("timer intr:\n");
    timer_clear();
    timer_next();
    return 1;
  }

  return 0;
}

void enable_intr(uint32 intr) {
  irq->enable[0] = intr;
}

void disable_intr(uint32 intr) {
  irq->disable[0] = intr;
}
