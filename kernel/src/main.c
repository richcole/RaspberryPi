#include "types.h"
#include "uart.h"
#include "gio.h"
#include "cpu.h"
#include "irq.h"
#include "framebuf.h"
#include "malloc.h"
#include "channel.h"
#include "font.h"

void task2() {
  while(1) {
    print_buf("Task 2\n");
    task_yield();
  }
}

int notmain ( void )
{
  uart_init();
  print_buf("\nBare Metal Programming For The Win!!\n");

  print_buf("\n Initializing.\n");

  malloc_init((void *)0x30000);
  framebuf_init();
  task_init();
  enable_irq();

  print_buf("\n Initialization complete.\n");
  draw_char('A', 0, 0, 0xffffffff);

  task_start(&task2);

  task_yield();
  print_buf("BBBB 1\n");
  task_yield();
  print_buf("AAAA 1\n");

  while(1);
  return 0;
}
