#include "core/pl011_uart.h"
#include "tasks.h"

void c_entry() {
  pl011_init();
  pl011_puts("Hello World!", 12);

  yield();
};
