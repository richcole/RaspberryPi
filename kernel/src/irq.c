#include "irq.h"
#include "uart.h"
#include "timer.h"

struct irq_t volatile *irq = (struct irq_t volatile *) 0x2000B200;

uint32 c_irq_handler() {
  if ( irq->pending[0] & uart_intr ) {
    uart_echo();
  }

  if ( irq->pending[0] & timer_intr ) {
    timer_clear();
    timer_next();
    return 1;
  }

  return 0;
}
