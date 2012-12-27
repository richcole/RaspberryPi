#include "framebuf.h"
#include "uart.h"

void draw_pattern() {
  uint8 volatile* ptr = framebuf_ptr();
  int x, y;
  uint32 width = framebuf_width();
  uint32 height = framebuf_height();
  uint32 pitch = framebuf_pitch();
  uint32 depth = framebuf_depth();
  for(y=0;y<height;++y) {
    for(x=0;x<width;++x) {
      ptr[(y*pitch)+(x*depth)+0] = y;
      ptr[(y*pitch)+(x*depth)+1] = x;
      ptr[(y*pitch)+(x*depth)+2] = x+y;
    }
  }
}

