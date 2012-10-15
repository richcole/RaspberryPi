
typedef unsigned short uint16;
typedef unsigned int   uint32;

extern void dummy ( uint32 );
extern uint32 spsr();
extern uint32 cpsr();
extern uint32 scr();

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

uint32 irq_disabled() {
  return spsr() & (0x1 << 7);
}

uint32 fiq_disabled() {
  return spsr() & (0x1 << 6);
}

uint32 mode() {
  return cpsr() & 0x1f;
}

#define MODE_USER  0b10000
#define MODE_FIQ   0b10001
#define MODE_IRQ   0b10010
#define MODE_SUPER 0b10011
#define MODE_ABORT 0b10111
#define MODE_UNDEF 0b11011
#define MODE_SYS   0b11111
#define MODE_MON   0b10110

char *mode_name(uint32 mode) {
  switch(mode) {
  case MODE_USER:  
    return "MODE_USER";
  case MODE_FIQ:   
    return "MODE_FIQ";
  case MODE_IRQ:   
    return "MODE_IRQ";
  case MODE_SUPER:   
    return "MODE_SUPER";
  case MODE_ABORT: 
    return "MODE_ABORT";
  case MODE_UNDEF: 
    return "MODE_UNDEF";
  case MODE_SYS:   
    return "MODE_SYS";
  case MODE_MON:   
    return "MODE_MON";
  }
  return "MODE_NONE";
}



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

void print_buf(char *buf) {
  for(;*buf;++buf) {
    while((uart->lsr&0x20) == 0);
    uart->io = *buf;
  }
}

void print_ch(char ch) {
  while((uart->lsr&0x20) == 0);
  uart->io = ch;
}

void print_hex(uint32 p) {
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
}

void print_addresses_neq(uint32 p, uint32 volatile *q, char *name) {
  print_hex(p);
  print_ch(' ');
  print_hex((uint32)q);
  print_ch(' ');
  print_buf(name);
  if ( p != (uint32)q ) {
    print_buf(" NOT EQUAL");
  }
  print_buf("\n");
}

int notmain ( void )
{
    init_uart();
    print_buf("\nBare Metal Programming For The Win!!\n");
    print_buf("spsr ");
    print_hex(spsr());
    print_buf("\n");

    print_buf("cpsr ");
    print_hex(cpsr());
    print_buf("\n");

    print_buf("scr ");
    print_hex(scr());
    print_buf("\n");

    print_buf("mode ");
    print_hex(mode());
    print_buf(" ");
    print_buf(mode_name(mode()));
    print_buf("\n");

    print_buf("IRQ ");
    print_hex(irq_disabled());
    print_buf("\n");

    print_buf("FIQ ");
    print_hex(fiq_disabled());
    print_buf("\n");

    while(1);
    return 0;
}
