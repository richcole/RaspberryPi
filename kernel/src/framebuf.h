#ifndef FRAMEBUF_H
#define FRAMEBUF_H

#include "types.h"

struct mailbox_t {
  uint32 read;
  uint32 pad1[3];
  uint32 peak;
  uint32 sender;
  uint32 status;
  uint32 config;
  uint32 write;
};

struct framebuf_info_t {
  uint32 width;
  uint32 height;
  uint32 vwidth;
  uint32 vheight;
  uint32 pitch;
  uint32 depth;
  uint32 xoffset;
  uint32 yoffset;
  uint32 ptr;
  uint32 size;
};

extern struct framebuf_info_t volatile *framebuf_info;
extern struct mailbox_t volatile *mailbox;
extern uint32 status_full;
extern uint32 status_empty;

void framebuf_init();

#endif
