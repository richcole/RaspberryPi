#ifndef CHANNEL_H
#define CHANNEL_H

#include "types.h"
#include "task.h"

struct channel_t {
  struct list_t *input_tasks;
  struct list_t *output_tasks;
};

void channel_send(struct channel_t *ch, struct buf_t *msg);
void channel_recv(struct channel_t *ch, struct buf_t *msg);

#endif
