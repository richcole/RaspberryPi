
extern void dummy ( unsigned int );

typedef unsigned short uint16;
typedef unsigned int   uint32;

struct gio_t {
  uint32 sel[6]; // 0x0
  uint32 res1;   // 0x18
  uint32 set[2]; // 0x1c
  uint32 res2;   // 0x24
  uint32 clr[2]; // 0x28
  uint32 res3;   // 0x34
  uint32 lev[2];
  uint32 res4;
  uint32 eds[2];
  uint32 res5;
  uint32 ren[2];
  uint32 res6;
  uint32 fen[2];
  uint32 res7;
  uint32 hen[2];
  uint32 res8;
  uint32 len[2];   // 0x70
  uint32 res9;
  uint32 aren[2];
  uint32 res10;
  uint32 afen[2];  // 0x88
  uint32 res11;
  uint32 pud;      // 0x94
  uint32 pudclk[2];
};

struct uart_t {
  uint32 irq;
  uint32 enable;
  uint32 pad[14];
  uint32 io;
  uint32 ier;
  uint32 iir;
  uint32 lcr;
  uint32 mcr;
  uint32 lsr;
  uint32 msr;
  uint32 scr;
  uint32 cntl;
  uint32 stat;
  uint32 baud;
};

void wait_cycles(int cycles) {
  while(cycles > 0) {
    dummy(cycles);
    --cycles;
  }

}

struct gio_t volatile *gio = (struct gio_t *)0x20200000;
struct uart_t volatile *uart = (struct uart_t *)0x20215000;

void init_uart() {

  uart->enable = 0x1;
  uart->ier    = 0;
  uart->cntl   = 0;
  uart->lcr    = 0x3;
  uart->mcr    = 0;
  uart->ier    = 0;
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

  // turn on the uart
  uart->cntl     = 2;
};

int print_nl(char *buf) {
  buf[0] = '\n';
  buf[1] = 0;
  return 1;
}

int print_hex(char *buf, unsigned int p) {
  int i;
  unsigned int c;
  buf[0] = '0';
  buf[1] = 'x';
  for(i=0;i<8;++i) {
    c = ((p>>(i*4))&0xf);
    if ( c < 10 ) {
      buf[9-i] = '0' + c;
    }
    else {
      buf[9-i] = 'A' + (c - 10);
    }
  };
  buf[10] = 0;
  return 10;
}

void print_buf(char *buf) {
  for(;*buf;++buf) {
    while((uart->lsr&0x20) == 0);
    uart->io = *buf;
  }
}

void print_addresses_neq(unsigned int p, uint32 volatile *q, char *name) {
  char buf[100];
  int  i = 0;
  i += print_hex(buf + i, p);
  buf[i++] = ' ';
  i += print_hex(buf + i, (unsigned int)q);
  buf[i++] = ' ';
  buf[i++] = 0;
  print_buf(buf);

  print_buf(name);
  if ( p != (unsigned int)q ) {
    print_buf(" NOT EQUAL");
  }
  print_buf("\n");
}

int notmain ( void )
{
    char *message = "\nBare Metal Programming For The Win!\n";
    init_uart();
    print_buf("\nBare Metal Programming For The Win!!\n");

    while(1);
    return 0;
}
