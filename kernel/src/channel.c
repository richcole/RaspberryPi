#include "channel.h"
#include "list.h"
#include "task.h"
#include "malloc.h"
#include "irq.h"
#include "uart.h"

void channel_transfer_to_output(
  struct task_t *current_task, 
  struct channel_t *ch
) {
  struct list_node_t *output_task_it = list_first(&ch->output_tasks);
  struct task_t *output_task = (struct task_t *)list_get(output_task_it);
  list_move_to_end(&ch->output_tasks, active_tasks, output_task_it);
  msg_move(current_task->msg, output_task->msg);
}

void channel_transfer_to_input(
  struct task_t *current_task, 
  struct channel_t *ch
) {
  struct list_node_t *input_task_it = list_first(&ch->input_tasks);
  struct task_t *input_task = (struct task_t *)list_get(input_task_it);
  list_move_to_end(&ch->input_tasks, active_tasks, input_task_it);
  msg_move(input_task->msg, current_task->msg);
}

void channel_send(struct channel_t *ch, struct msg_t *msg) {
  print_buf("channel_send: begin\n");
  disable_irq();
  struct task_t *current_task = (struct task_t *)current_task_it->data;
  current_task->msg = msg;
  if ( list_empty(&ch->output_tasks) ) {
    task_yield_and_move_to(&ch->input_tasks);
  }
  else {
    channel_transfer_to_output(current_task, ch);
  }
  print_buf("channel_send: end\n");
  enable_irq();
}

void channel_recv(struct channel_t *ch, struct msg_t *msg) {
  print_buf("channel_send: begin\n");
  disable_irq();
  struct task_t *current_task = (struct task_t *)current_task_it->data;
  current_task->msg = msg;
  if ( list_empty(&ch->input_tasks) ) {
    task_yield_and_move_to(&ch->output_tasks);
  }
  else {
    channel_transfer_to_input(current_task, ch);
  }
  print_buf("channel_send: end\n");
  enable_irq();
}

void channel_init(struct channel_t *ch) {
  list_init(&ch->input_tasks);
  list_init(&ch->output_tasks);
};

struct channel_t *channel_new() {
  struct channel_t* ch = (struct channel_t *)
    malloc_alloc(sizeof(struct channel_t));
  channel_init(ch);
  return ch;
}
