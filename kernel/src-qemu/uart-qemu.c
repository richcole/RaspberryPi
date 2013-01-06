#include "uart.h"
#include "gio.h"
#include "cpu.h"
#include "irq.h"

#include <stdint.h>

typedef struct uart_t {
 uint32_t DR;
 uint32_t RSR_ECR;
 uint8_t reserved1[0x10];
 const uint32_t FR;
 uint8_t reserved2[0x4];
 uint32_t LPR;
 uint32_t IBRD;
 uint32_t FBRD;
 uint32_t LCR_H;
 uint32_t CR;
 uint32_t IFLS;
 uint32_t IMSC;
 const uint32_t RIS;
 const uint32_t MIS;
 uint32_t ICR;
 uint32_t DMACR;
} uart_t;

uint32 RXFE = 0x10;
uint32 TXFF = 0x20;

struct uart_t volatile *uart = (struct uart_t volatile *)0x101f1000;
uint32 uart_intr = 0x1 << 29;

void uart_init() {
};

void print_buf(char *buf) {
  for(;*buf;++buf) {
    uart->DR = *buf;
  }
}

void print_ch(char ch) {
  uart->DR = ch;
}

void uart_echo() {
  while ((uart->FR & RXFE) == 0) {
    uart->DR = uart->DR;
  }
}

