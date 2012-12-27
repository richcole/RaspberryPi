#ifndef CHANNEL_H
#define CHANNEL_H

#include "types.h"
#include "task.h"

typedef struct channel_t {
  struct list_t input_tasks;
  struct list_t output_tasks;
} channel_t;

void channel_send(struct channel_t *ch, struct msg_t *msg);
void channel_recv(struct channel_t *ch, struct msg_t *msg);
struct channel_t *channel_new();
void channel_init(struct channel_t *ch);

#endif
