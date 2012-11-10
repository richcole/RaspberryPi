#include "buf.h"
#include "malloc.h"

void buf_new(struct buf_t *buf, uint32 size, uint32 type) {
  buf->data = malloc_alloc(size);
  buf->size = size;
  buf->type = type;
}

void buf_move(struct buf_t *src, struct buf_t *dst) {
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
