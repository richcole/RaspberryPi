#include "irq.h"
#include "uart.h"
#include "timer.h"

struct irq_t volatile *irq = (struct irq_t volatile *) 0x2000B200;

uint32 c_irq_handler() {
  unsigned int rb;
  unsigned int x;

  print_buf("intr pending=");
  print_hex(irq->pending[0]);
  print_buf("\n");

  if ( irq->pending[0] & uart_intr ) {
    while(((rb = uart->iir)&0x1) == 0) {
      if ((rb&0x6)==4) {
        x = uart->io;
        print_ch(x);
      }
    }
  }

  if ( irq->pending[0] & timer_intr ) {
    timer_clear();
    timer_next();
    return 1;
  }

  return 0;
}
