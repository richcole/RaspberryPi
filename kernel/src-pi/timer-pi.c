#include "types.h"
#include "irq.h"

typedef struct timer_t {
  uint32 cs;
  uint32 clo;
  uint32 chi;
  uint32 c[4];
} timer_t;

struct timer_t volatile *timer = (struct timer_t volatile *)0x20003000;
uint32 timer_intr = 0x2;

void timer_init() {
  timer->c[1] = timer->clo + 0x100000;
  enable_intr(timer_intr);
};

void timer_next() {
  timer->c[1] = timer->clo + 0x100000;
};

void timer_clear() {
  timer->cs = 0x2;
}
