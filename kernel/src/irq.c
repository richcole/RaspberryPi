#include "irq.h"
#include "uart.h"

uint32 volatile *irq_enable  = (unsigned int *)0x2000B210;
uint32 volatile *irq_disable = (unsigned int *)0x2000B21C;

void c_irq_handler() {
  unsigned int rb;
  unsigned int x;

  while(((rb = uart->iir)&0x1) == 0) {
    if ((rb&0x6)==4) {
      x = uart->io;
      print_ch(x);
    }
  }
}
