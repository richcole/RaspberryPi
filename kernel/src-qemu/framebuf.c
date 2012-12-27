#include "framebuf.h"
#include "uart.h"

void framebuf_init() {
};

uint8 volatile* framebuf_ptr() {
  return (uint8 volatile *)0x100000;
};

uint32 framebuf_height() {
  return 0;
};

uint32 framebuf_width() {
  return 0;
};

uint32 framebuf_depth() {
  return 0;
};

uint32 framebuf_pitch() {
  return 0;
};
