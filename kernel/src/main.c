
#include "types.h"
#include "uart.h"
#include "gio.h"
#include "cpu.h"
#include "irq.h"
#include "framebuf.h"

int notmain ( void )
{
  *irq_disable = uart_intr;
  uart_init();

  print_buf("\nBare Metal Programming For The Win!!\n");
  print_buf("spsr ");
  print_hex(spsr());
  print_buf("\n");
  
  print_buf("cpsr ");
  print_hex(cpsr());
  print_buf("\n");
  
  print_buf("c1 ");
  print_hex(c1());
  print_buf("\n");
  
  print_buf("mode ");
  print_hex(mode());
  print_buf(" ");
  print_buf(mode_name(mode()));
  print_buf("\n");
  
  print_buf("IRQ ");
  print_hex(irq_disabled());
  print_buf("\n");
  
  print_buf("FIQ ");
  print_hex(fiq_disabled());
  print_buf("\n");
  
  print_buf("LSR ");
  print_hex(uart->lsr);
  print_buf("\n");

  *irq_enable = uart_intr;
  enable_irq();
  
  print_buf("IRQ ");
  print_hex(irq_disabled());
  print_buf("\n");

  framebuf_init();
 
  while(1);
  
  return 0;
}
