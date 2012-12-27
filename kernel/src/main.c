#include "types.h"
#include "uart.h"
#include "gio.h"
#include "cpu.h"
#include "irq.h"
#include "framebuf.h"
#include "malloc.h"
#include "channel.h"
#include "font.h"
#include "timer.h"
#include "channel.h"
#include "msg.h"

struct channel_t ch;
struct msg_t msg1;
struct msg_t msg2;

void task2() {
  while(1) {
    print_buf("Task 2\n");
    channel_send(&ch, &msg2);
  }
}

void idle() {
  print_buf("Idle task started\n");
  while(1) {
    task_yield();
  }
}

int notmain ( void )
{
  uart_init();
  print_buf("\nBare Metal Programming For The Win!!\n");

  print_buf("\n Initializing.\n");

  malloc_init((void *)0x50000);
  framebuf_init();
  task_init();
  timer_init();

  print_buf("\n Initialization complete.\n");
  draw_char('A', 0, 0, 0xffffffff);

  channel_init(&ch);
  msg_init(&msg1, 1, 1);
  msg_init(&msg2, 1, 1);

  task_start(&idle);
  task_start(&task2); 
  
  while(1) {
    print_buf("Task 1\n");
    channel_recv(&ch, &msg1);
  }
  return 0;
}
