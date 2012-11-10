#ifndef buf_h
#define buf_h

#include "types.h"

struct buf_t {
  uint32 size;
  uint32 type;
  char   *data;
};

void buf_new(struct buf_t *buf, uint32 size, uint32 type);
void buf_move(struct buf_t *src, struct buf_t *dst);

#endif
