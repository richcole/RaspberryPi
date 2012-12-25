#ifndef UART_H
#define UART_H

#include "types.h"

struct uart_t {
  uint32 irq;      // 0
  uint32 enable;   // 4
  uint32 pad[14];  // 8
  uint32 io;       // 40
  uint32 ier;      // 44
  uint32 iir;      // 48
  uint32 lcr;      // 4C
  uint32 mcr;      // 50
  uint32 lsr;      // 54
  uint32 msr;      // 58
  uint32 scr;      // 5C
  uint32 cntl;     // 60
  uint32 stat;     // 64
  uint32 baud;     // 68
};

extern struct uart_t volatile *uart;

void uart_init();
void print_buf(char *buf);
void print_ch(char ch);
uint32 print_hex(uint32 p);
void volatile* print_ptr(void *p);

extern uint32 uart_intr;

#endif
