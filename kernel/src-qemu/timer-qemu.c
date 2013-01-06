#include "types.h"
#include "irq.h"

typedef struct timer_t {
  uint32 load;
  uint32 value;
  uint32 control;
  uint32 intr_clear;
  uint32 ris;
  uint32 mis;
  uint32 bgload;
} timer_t;

struct timer_t;
struct timer_t volatile *timer = (struct timer_t volatile *)0x101E2000;
uint32 timer_intr = 0x4;

void timer_init() {
  timer->control = 0b1010000;
  timer->load = 0x100000;
  enable_intr(timer_intr);
};

void timer_next() {
  timer->load = 0x100000;
};

void timer_clear() {
  timer->intr_clear = 0;
}
