#ifndef UART_H
#define UART_H

#include "types.h"

void uart_init();
void print_buf(char *buf);
void print_ch(char ch);
uint32 print_hex(uint32 p);
void volatile* print_ptr(void *p);
void uart_echo();

extern uint32 uart_intr;
extern struct uart_t volatile *uart;

#endif
