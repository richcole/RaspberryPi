#include "uart.h"

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

void volatile* print_ptr(void *p) {
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
