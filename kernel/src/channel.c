#include "channel.h"
#include "list.h"

void channel_transfer(struct channel_t *ch, struct buf_t *buf, 
  struct list_node_t *to_ch_it) {
}

void channel_send(struct channel_t *ch, struct buf_t *msg) {
  struct task_t *current_task = get_current_task();
  current_task->msg = msg;
  if ( list_empty(ch->output_tasks) ) {
    list_move_to_end(active_tasks, ch->input_tasks, current_task_it);
  }
  else {
    channel_transfer(ch, msg, list_first(ch->output_tasks));
  }
}



  
