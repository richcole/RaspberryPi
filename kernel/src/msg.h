#ifndef buf_h
#define buf_h

#include "types.h"

typedef struct msg_t {
  uint32 size;
  uint32 type;
  char   *data;
} msg_t;

void msg_init(struct msg_t *buf, uint32 type, uint32 size);
void msg_move(struct msg_t *src, struct msg_t *dst);

#define cast(p, cst) ((cst)(p))

#endif
