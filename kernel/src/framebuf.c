#include "framebuf.h"
#include "uart.h"

struct framebuf_info_t volatile framebuf_info_s __attribute__((aligned (16)));
struct framebuf_info_t volatile *framebuf_info = &framebuf_info_s;
unsigned char *framebuf_ptr = 0;

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
  framebuf_info->width = 1024;
  framebuf_info->height = 768;
  framebuf_info->vwidth = 1024;
  framebuf_info->vheight = 768;
  framebuf_info->pitch = 0;
  framebuf_info->depth = 24;
  framebuf_info->xoffset = 0;
  framebuf_info->yoffset = 0;
  framebuf_info->ptr = 0;
  framebuf_info->size = 0;

  print_buf("Write to Mailbox\n");
  print_buf("framebuf: ");
  print_hex((uint32)framebuf_info);
  mailbox_write(1, (uint32)framebuf_info);
  uint32 reply = 0;
  reply = mailbox_read(1);
  print_buf("\nreply: ");
  print_hex(reply);
  print_buf("\nptr: ");
  print_hex((uint32)framebuf_info->ptr);
  print_buf("\nsize: ");
  print_hex((uint32)framebuf_info->size);
  print_buf("\npitch: ");
  print_hex((uint32)framebuf_info->pitch);
  print_buf("\ndepth: ");
  print_hex((uint32)framebuf_info->depth);
  print_buf("\nwidth: ");
  print_hex((uint32)framebuf_info->width);
  print_buf("\nheight: ");
  print_hex((uint32)framebuf_info->height);
  print_buf("\n");

  framebuf_ptr = (unsigned char *)framebuf_info->ptr;

  int x, y;
  uint32 width = framebuf_info->width;
  uint32 height = framebuf_info->height;
  uint32 pitch = framebuf_info->pitch;
  uint32 depth = framebuf_info->depth / 8;
  if ( pitch == 0 ) {
    pitch = depth * width;
  }
  for(y=0;y<height;++y) {
    for(x=0;x<width;++x) {
      framebuf_ptr[(y*pitch)+(x*depth)+0] = y;
      framebuf_ptr[(y*pitch)+(x*depth)+1] = x;
      framebuf_ptr[(y*pitch)+(x*depth)+2] = x+y;
    }
  }
};
  
