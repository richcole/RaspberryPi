#ifndef PL011_UART_H
#define PL011_UART_H

#include "stdint.h"

typedef struct {
  uint32_t  _baud;
  uint8_t   _data_bits;
  uint8_t   _parity;
  uint8_t   _stop_bits;
} pl011_config_t;

void     pl011_putc(uint8_t c);
void     pl011_puts(uint8_t * c, size_t count);

uint8_t  pl011_getc();
uint8_t  pl011_peekc();
size_t   pl011_gets(uint8_t * string, size_t count);

void     pl011_init();
void     pl011_open();
void     pl011_close();
void     pl011_flush();

uint32_t pl011_configure(pl011_config_t *);

#endif
