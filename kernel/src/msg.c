#include "msg.h"
#include "malloc.h"

void msg_new(struct msg_t *buf, uint32 size, uint32 type) {
  buf->data = malloc_alloc(size);
  buf->size = size;
  buf->type = type;
}

void msg_move(struct msg_t *src, struct msg_t *dst) {
  if ( dst->data != 0 ) {
    malloc_free(dst->data);
  }
  dst->data = src->data;
  dst->size = src->size;
  dst->type = src->type;
  src->type = 0;
  src->size = 0;
  src->data = 0;
}
