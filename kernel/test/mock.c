#include "types.h"

uint32 c1() {
}

uint32 cpsr() {
  return 0;
}

uint32 spsr() {
  return 0;
}

uint32 dummy() {
  return 0;
}

void enable_irq() {
}

void set_if_zero(uint32 *ptr, uint32 value) {
  *ptr = value;
}

void switch_to(struct task_t *task) {
}

