#include "framebuf.h"
#include "uart.h"

typedef struct mailbox_t {
  uint32 read;
  uint32 pad1[3];
  uint32 peak;
  uint32 sender;
  uint32 status;
  uint32 config;
  uint32 write;
} mailbox_t;

typedef struct framebuf_info_t {
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
} framebuf_info_t;

struct framebuf_info_t volatile framebuf_info __attribute__((aligned (16)));

struct mailbox_t volatile *mailbox = (struct mailbox_t *)0x2000B880;
uint32 status_full  = 0x1 << 31;
uint32 status_empty = 0x1 << 30;

void mailbox_write(uint32 chan, uint32 data) {
  while(mailbox->status & status_full);
  mailbox->write = (chan & 0xf) | (data & ~(uint32)0xf);
}

uint32 mailbox_read(uint32 chan) {
  uint32 val;
  do {
    while(mailbox->status & status_empty);
    val = mailbox->read;
  }
  while((val & 0xf) != chan);
  return val;
}

void framebuf_init() {
  framebuf_info.width = 1024;
  framebuf_info.height = 768;
  framebuf_info.vwidth = 1024;
  framebuf_info.vheight = 768;
  framebuf_info.pitch = 0;
  framebuf_info.depth = 24;
  framebuf_info.xoffset = 0;
  framebuf_info.yoffset = 0;
  framebuf_info.ptr = 0;
  framebuf_info.size = 0;

  mailbox_write(1, ((uint32)&framebuf_info) + 0x40000000);
  mailbox_read(1);
};

uint8 volatile* framebuf_ptr() {
  return (uint8 volatile*)framebuf_info.ptr;
}

uint32 framebuf_width() {
  return framebuf_info.width;
}

uint32 framebuf_height() {
  return framebuf_info.height;
}

uint32 framebuf_depth() {
  return framebuf_info.depth / 8;
}

uint32 framebuf_pitch() {
  return framebuf_info.pitch;
}
