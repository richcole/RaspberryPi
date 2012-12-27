#ifndef FRAMEBUF_H
#define FRAMEBUF_H

#include "types.h"

void framebuf_init();
uint8 volatile* framebuf_ptr();
uint32 framebuf_height();
uint32 framebuf_width();
uint32 framebuf_depth();
uint32 framebuf_pitch();

void draw_pattern();


#endif
