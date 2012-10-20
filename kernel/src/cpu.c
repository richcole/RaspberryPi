#include "cpu.h"

#define MODE_USER  0b10000
#define MODE_FIQ   0b10001
#define MODE_IRQ   0b10010
#define MODE_SUPER 0b10011
#define MODE_ABORT 0b10111
#define MODE_UNDEF 0b11011
#define MODE_SYS   0b11111
#define MODE_MON   0b10110

void wait_cycles(uint32 cycles) {
  while(cycles > 0) {
    dummy(cycles);
    --cycles;
  }

}

uint32 irq_disabled() {
  return cpsr() & (0x1 << 7);
}

uint32 fiq_disabled() {
  return cpsr() & (0x1 << 6);
}

uint32 mode() {
  return cpsr() & 0x1f;
}

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

