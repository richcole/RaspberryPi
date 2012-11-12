#include "channel.h"
#include "list.h"
#include "task.h"

extern void switch_to(uint32 *from_sp_ptr, uint32 to_sp);

void channel_transfer_to_output(
  struct task_t *current_task, 
  struct channel_t *ch, 
  struct msg_t *msg
) {
  struct list_node_t *output_task_it = list_first(ch->output_tasks);
  struct task_t *output_task = (struct task_t *)list_get(output_task_it);
  list_move_to_end(ch->output_tasks, active_tasks, output_task_it);
  msg_move(current_task->msg, output_task->msg);
  current_task_it = otuput_task_it;
  switch_to(&current_task->sp, output_task->sp);
}

void channel_transfer_to_input(
  struct task_t *current_task, 
  struct channel_t *ch, 
  struct msg_t *msg
) {
  struct list_node_t *input_task_it = list_first(ch->input_tasks);
  struct task_t *input_task = (struct task_t *)list_get(input_task_it);
  list_move_to_end(ch->input_tasks, active_tasks, input_task_it);
  msg_move(input_task->msg, current_task->msg);
}

void channel_send(struct channel_t *ch, struct msg_t *msg) {
  struct task_t *current_task = task_current();
  current_task->msg = msg;
  if ( list_empty(ch->output_tasks) ) {
    list_move_to_end(active_tasks, ch->input_tasks, current_task_it);
    task_yield();
  }
  else {
    channel_transfer_to_output(current_task, ch, msg);
  }
}

void channel_recv(struct channel_t *ch, struct msg_t *msg) {
  struct task_t *current_task = task_current();
  if ( list_empty(ch->input_tasks) ) {
    list_move_to_end(active_tasks, ch->output_tasks, current_task_it);
    task_yield();
  }
  else {
    channel_transfer_to_input(current_task, ch, msg);
  }
}


  
