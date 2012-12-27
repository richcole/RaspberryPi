#include "uart.h"
#include "gio.h"
#include "cpu.h"
#include "irq.h"

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

struct uart_t volatile *uart = (struct uart_t *)0x20215000;
uint32 uart_intr = 0x1 << 29;

void uart_init() {

  irq->disable[0] = uart_intr;

  uart->enable = 0x1;
  uart->ier    = 0;
  uart->cntl   = 0;
  uart->lcr    = 0x3;
  uart->mcr    = 0;
  uart->ier    = 0x5;
  uart->iir    = 0xC6;
  uart->baud   = 270;

  // pin 13 -> b000 = input 
  // pin 14 -> b001 = output 
  gio->sel[1]    = (gio->sel[1] & ~(0x7 << 12)) | (0x1<<13);

  // set pull updown to off 
  gio->pud       = 0;
  wait_cycles(150);

  // enable clk on 13 to take the pud state
  gio->pudclk[0] = (0x1 << 14);
  wait_cycles(150);

  // disable clk as we've finished modifying the pud state 
  gio->pudclk[0] = 0;

  // turn on the uart for send and receive
  uart->cntl     = 3;

  irq->enable[0] = uart_intr;
};

void print_buf(char *buf) {
  for(;*buf;++buf) {
    while((uart->lsr&0x20) == 0);
    uart->io = *buf;
  }
}

void print_ch(char ch) {
  while((uart->lsr&0x20) == 0);
  uart->io = ch;
  while((uart->lsr&0x20) == 0);
}

void uart_echo() {
  uint32 rb;
  while(((rb = uart->iir)&0x1) == 0) {
    if ((rb&0x6)==4) {
      print_ch(uart->io);
    }
  }
}

