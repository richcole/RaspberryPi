#ifndef CHANNEL_H
#define CHANNEL_H

#include "types.h"
#include "task.h"

struct channel_t {
  struct list_t input_tasks;
  struct list_t output_tasks;
  uint32 closed;
};

struct mchannel_t {
  struct list_t      recv;
  struct list_t      send;
  struct msg_t*      msg;
  struct channel_t*  channel;
};

uint32 channel_send(struct channel_t *ch, struct msg_t *msg);
uint32 channel_recv(struct channel_t *ch, struct msg_t *msg);
void channel_close(struct channel_t *ch);

struct channel_t *channel_new();
void channel_init(struct channel_t *ch);

void mchannel_init(struct mchannel_t *ch, struct msg_t *msg);

#endif
