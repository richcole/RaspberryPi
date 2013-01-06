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

typedef struct counter_msg_t {
  struct msg_t msg;
  uint32 counter;
} counter_msg_t;

void task2() {
  struct counter_msg_t msg;
  msg_init(&msg.msg, 1, sizeof(struct counter_msg_t));
  msg.counter = 0;
  while(1) {
    ++msg.counter;
    print_buf("Task 2\n");
    channel_send(&ch, &msg.msg);
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
  struct counter_msg_t msg;

  uart_init();
  print_buf("\nBare Metal Programming For The Win!!\n");

  print_buf("\n Initializing.\n");

  malloc_init((void *)0x50000);
  framebuf_init();
  task_init();
  timer_init();
  channel_init(&ch);

  print_buf("\n Initialization complete.\n");
  draw_char('A', 0, 0, 0xffffffff);

  task_start(&idle);
  task_start(&task2);

  print_buf("Tasks created\n");

  msg_init(&msg.msg, 0, sizeof(struct counter_msg_t));
  msg.counter = -1;
  while(1) {
    channel_recv(&ch, &msg.msg);
    print_buf("Task 1 counter=");
    print_hex(msg.counter);
    print_buf(" type=");
    print_hex(msg.msg.type);
    print_buf("\n");
  }
  return 0;
}
