#include "msg.h"
#include "malloc.h"

void msg_init(struct msg_t *buf, uint32 type, uint32 size) {
  buf->data = size ? malloc_alloc(size) : 0;
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
