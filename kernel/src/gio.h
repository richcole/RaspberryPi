#ifndef GIO_H
#define GIO_H

#include "types.h"

struct gio_t {
  uint32 sel[6]; // 0x0
  uint32 res1;   // 0x18
  uint32 set[2]; // 0x1c
  uint32 res2;   // 0x24
  uint32 clr[2]; // 0x28
  uint32 res3;   // 0x34
  uint32 lev[2];
  uint32 res4;
  uint32 eds[2];
  uint32 res5;
  uint32 ren[2];
  uint32 res6;
  uint32 fen[2];
  uint32 res7;
  uint32 hen[2];
  uint32 res8;
  uint32 len[2];   // 0x70
  uint32 res9;
  uint32 aren[2];
  uint32 res10;
  uint32 afen[2];  // 0x88
  uint32 res11;
  uint32 pud;      // 0x94
  uint32 pudclk[2];
};

extern struct gio_t volatile *gio;

#endif
