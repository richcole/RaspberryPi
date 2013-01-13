#include "debug.h"
#include "uart.h"

void debug(char *s) {
  print_buf(s);
}

void debug_ptr(void *p) {
  print_buf(p);
}

void debug_uint32(uint32 i) {
  print_hex(i);
}
