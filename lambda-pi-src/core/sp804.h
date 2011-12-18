#ifndef SP804_H
#define SP804_H
/*        Copyright (c) 20011, Simon Stapleton (simon.stapleton@gmail.com)        */
/*                                                                                */
/*                              All rights reserved.                              */
/*                                                                                */
/* Redistribution  and  use   in  source  and  binary  forms,   with  or  without */
/* modification, are permitted provided that the following conditions are met:    */
/*                                                                                */
/* Redistributions of  source code must  retain the above copyright  notice, this */
/* list of conditions and the following disclaimer.                               */
/*                                                                                */
/* Redistributions in binary form must reproduce the above copyright notice, this */
/* list of conditions and the following disclaimer in the documentation and/or    */
/* other materials provided with the distribution.                                */
/*                                                                                */
/* Neither the name of  the developer nor the names of  other contributors may be */
/* used  to  endorse or  promote  products  derived  from this  software  without */
/* specific prior written permission.                                             */
/*                                                                                */
/* THIS SOFTWARE  IS PROVIDED BY THE  COPYRIGHT HOLDERS AND CONTRIBUTORS  "AS IS" */
/* AND ANY  EXPRESS OR  IMPLIED WARRANTIES,  INCLUDING, BUT  NOT LIMITED  TO, THE */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE */
/* DISCLAIMED. IN NO  EVENT SHALL THE COPYRIGHT HOLDER OR  CONTRIBUTORS BE LIABLE */
/* FOR  ANY DIRECT,  INDIRECT, INCIDENTAL,  SPECIAL, EXEMPLARY,  OR CONSEQUENTIAL */
/* DAMAGES (INCLUDING,  BUT NOT  LIMITED TO, PROCUREMENT  OF SUBSTITUTE  GOODS OR */
/* SERVICES; LOSS  OF USE,  DATA, OR PROFITS;  OR BUSINESS  INTERRUPTION) HOWEVER */
/* CAUSED AND ON ANY THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING  IN ANY WAY OUT OF THE USE */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.           */

#include "platform.h"

// functions and defines for a generic SP804 dual timer peripheral
// See http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0271d/index.html
#define SP804_XLOAD     0x00
#define SP804_XVALUE    0x04
#define SP804_XCONTROL  0x08
#define SP804_XINTCLR   0x0c
#define SP804_XRIS      0x10
#define SP804_XMIS      0x14
#define SP804_XBGLOAD   0x18

// Mode bits
#define SP804_ENABLE    0x00000080
// Periodic / freerunning
#define SP804_PERIODIC  0x00000040
#define SP804_FREERUN   0x00000000
// Generation of interrupts
#define SP804_INT_ENBL  0x00000020
#define SP804_INT_DIBL  0x00000000
// Prescaler
#define SP804_PRE_1     0x00000000
#define SP804_PRE_16    0x00000004
#define SP804_PRE_256   0x00000008
// Size
#define SP804_SIZE_32   0x00000002
#define SP804_SIZE_16   0x00000000
// Wrap mode
#define SP804_ONESHOT   0x00000001
#define SP804_WRAP      0x00000000

static inline void sp804_set_mode(uint32_t timer_base, uint32_t mode) {
  uint32_t curmode = *REG32(timer_base + SP804_XCONTROL);
  curmode &= !SP804_ENABLE;                 // Clear enable bit
  *(REG32(timer_base + SP804_XCONTROL)) = curmode; // Disable timer
  DSB;
  *(REG32(timer_base + SP804_XCONTROL)) = mode;    // And set mode
}

static inline int sp804_masked_status(uint32_t timer_base) {
  return *(REG32(timer_base + SP804_XMIS));
}

static inline void sp804_clear_interrupt(uint32_t timer_base) {
  *(REG32(timer_base + SP804_XINTCLR)) = 0x00000000;
}  

static inline void sp804_set_bg_load_value(uint32_t timer_base, uint32_t value) {
  *(REG32(timer_base + SP804_XBGLOAD)) = value;
}

static inline void sp804_set_load_value(uint32_t timer_base, uint32_t value) {
  *(REG32(timer_base + SP804_XLOAD)) = value;
}

static inline uint32_t sp804_value(uint32_t timer_base) {
  return *(REG32(timer_base + SP804_XVALUE));
}

#endif /* end of include guard: SP804_H */
