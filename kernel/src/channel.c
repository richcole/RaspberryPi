#include "channel.h"
#include "list.h"
#include "task.h"
#include "malloc.h"
#include "irq.h"
#include "uart.h"

void channel_activate_task(struct task_t *task);

void channel_transfer_to_output(
  struct channel_t *ch
) {
  struct task_t *current_task = task_current();
  struct list_node_t *output_task_it = list_first(&ch->output_tasks);
  struct task_t *output_task = (struct task_t *)list_get(output_task_it);
  channel_activate_task(output_task);
  msg_move(current_task->msg, output_task->msg);
}

void channel_transfer_to_input(
  struct channel_t *ch
) {
  struct task_t *current_task = task_current();
  struct list_node_t *input_task_it = list_first(&ch->input_tasks);
  struct task_t *input_task = (struct task_t *)list_get(input_task_it);
  channel_activate_task(input_task);
  msg_move(input_task->msg, current_task->msg);
}

void channel_send(struct channel_t *ch, struct msg_t *msg) {
  print_buf("channel_send: begin\n");
  disable_irq();
  task_set_msg(msg);
  if ( list_empty(&ch->output_tasks) ) {
    struct task_t *current_task = task_make_inactive();
    list_add_last(&ch->input_tasks, current_task);
    list_add_last(&current_task->input_channels, ch);
    task_yield();
  }
  else {
    channel_transfer_to_output(ch);
  }
  print_buf("channel_send: end\n");
  enable_irq();
}

void channel_recv(struct channel_t *ch, struct msg_t *msg) {
  print_buf("channel_recv: begin\n");
  disable_irq();
  task_set_msg(msg);
  if ( list_empty(&ch->input_tasks) ) {
    struct task_t *current_task = task_make_inactive();
    list_add_last(&ch->output_tasks, current_task);
    list_add_last(&current_task->output_channels, ch);
    task_yield();
  }
  else {
    channel_transfer_to_input(ch);
  }
  print_buf("channel_recv: end\n");
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

void mchannel_init(struct mchannel_t *mc, struct msg_t *msg) {
  list_init(&mc->send);
  list_init(&mc->recv);
  mc->msg = msg;
  mc->channel = 0;
}

void mchannel_add_recv(struct mchannel_t *mc, struct channel_t *ch) {
  list_add_last(&mc->recv, ch);
}

void mchannel_add_send(struct mchannel_t *mc, struct channel_t *ch) {
  list_add_last(&mc->send, ch);
}

void mchannel_set_msg(struct mchannel_t *mc, struct msg_t *msg) {
  mc->msg = msg;
}

void mchannel_act(struct mchannel_t *mc) {
  disable_irq();

  FOR_LIST_BEGIN(it, ch, struct channel_t *, &mc->recv);
  if ( ! list_empty(&ch->output_tasks) ) {
    channel_transfer_to_input(ch);
    mc->channel = ch;
    enable_irq();
    return;
  }
  FOR_LIST_END();

  FOR_LIST_BEGIN(it, ch, struct channel_t *, &mc->send);
  if ( ! list_empty(&ch->output_tasks) ) {
    channel_transfer_to_output(ch);
    mc->channel = ch;
    enable_irq();
    return;
  }
  FOR_LIST_END();

  // no channel is ready, need to queue
  struct task_t *current_task = task_make_inactive();

  FOR_LIST_BEGIN(it, ch, struct channel_t *, &mc->recv);
  list_add_last(&ch->input_tasks, current_task);
  task_add_input_channel(current_task, ch);
  FOR_LIST_END();

  FOR_LIST_BEGIN(it, ch, struct channel_t *, &mc->send);
  list_add_last(&ch->output_tasks, current_task);
  task_add_output_channel(current_task, ch);
  FOR_LIST_END();
  
  task_yield();
}

void channel_activate_task(struct task_t *task) {
  print_buf("channel_active_task:\n");
  FOR_LIST_BEGIN(it, ch, struct channel_t *, &task->input_channels);
  print_buf("remove task from input channel: ch=");
  print_ptr(ch);
  print_buf("\n");
  list_remove_value(&ch->input_tasks, task);
  FOR_LIST_END();

  FOR_LIST_BEGIN(it, ch, struct channel_t *, &task->output_channels);
  print_buf("remove task from output channel: ch=");
  print_ptr(ch);
  print_buf("\n");
  list_remove_value(&ch->output_tasks, task);
  FOR_LIST_END();

  list_remove_all(&task->output_channels);
  list_remove_all(&task->input_channels);
  task_make_active(task);
}


