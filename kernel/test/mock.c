#include "types.h"
#include "task.h"

uint32 __bss_end__ = 0;

uint32 sp() {
  return 0;
}

uint32 c1() {
  return 0;
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

void task_create(void *sp, task_func_t *lr) {
}

void switch_to_no_save(void *sp) {
}

void switch_to(void *sp) {
}
