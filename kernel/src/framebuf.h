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

void framebuf_init();
extern unsigned char volatile *framebuf_ptr;

#endif
