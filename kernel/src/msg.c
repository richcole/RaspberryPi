#include "msg.h"
#include "malloc.h"
#include "uart.h"

void msg_init(struct msg_t *msg, uint32 type, uint32 size) {
  msg->size = size - sizeof(struct msg_t);
  msg->type = type;
}

void msg_move(struct msg_t *src, struct msg_t *dst) {
  uint32 len = src->size;
  if ( dst->size < src->size ) {
    len = dst->size;
  }
  memcpy(src->data, dst->data, len);
  dst->type = src->type;
}

void msg_close(struct msg_t *msg) {
  msg->type = 0;
}

uint32 msg_is_closed(struct msg_t *msg) {
  return msg->type == 0;
}
