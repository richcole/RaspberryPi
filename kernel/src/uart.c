#include "uart.h"
#include "gio.h"
#include "cpu.h"
#include "irq.h"

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

uint32 print_hex(uint32 p) {
  int i;
  uint32 c;
  print_ch('0');
  print_ch('x');
  for(i=0;i<8;++i) {
    c = ((p>>((7-i)*4))&0xf);
    if ( c < 10 ) {
      print_ch('0' + c);
    }
    else {
      print_ch('A' + c - 10);
    }
  };
  return p;
}

void volatile* print_ptr(void volatile* p) {
  int i;
  uint32 c;
  int len = sizeof(p) * 2;
  unsigned long l = (unsigned long)p;
  print_ch('0');
  print_ch('x');
  for(i=0;i<len;++i) {
    c = ((l>>((len-i-1)*4))&0xf);
    if ( c < 10 ) {
      print_ch('0' + c);
    }
    else {
      print_ch('A' + c - 10);
    }
  };
  return p;
}

void print_addresses_neq(void *p, void volatile *q, char *name) {
  print_ptr(p);
  print_ch(' ');
  print_ptr(q);
  print_ch(' ');
  print_buf(name);
  if ( p != q ) {
    print_buf(" NOT EQUAL");
  }
  print_buf("\n");
}

