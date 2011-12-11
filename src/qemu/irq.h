#ifndef IRQ_H_QEMU
#define IRQ_H_QEMU
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

#include "bsp.h"

// Defines for the pl190 VIC emulated by qemu
#define VIC_BASE    0x10140000
#define IRQ_STATUS  REG32(VIC_BASE + 0x00)
#define FIQ_STATUS  REG32(VIC_BASE + 0x04)
#define INT_SELECT  REG32(VIC_BASE + 0x0c)
#define INT_ENABLE  REG32(VIC_BASE + 0x10)
#define INT_DISABLE REG32(VIC_BASE + 0x14)
#define INT_SOFT    REG32(VIC_BASE + 0x18)
#define INT_ACK     REG32(VIC_BASE + 0x30)

#define INTERRUPT_WDOG      0
#define INTERRUPT_SWI       1
#define INTERRUPT_DEBUG_RX  2
#define INTERRUPT_DEBUG_TX  3
#define INTERRUPT_TIMER12   4
#define INTERRUPT_TIMER34   5
#define INTERRUPT_GPIO0     6
#define INTERRUPT_GPIO1     7
#define INTERRUPT_GPIO2     8
#define INTERRUPT_GPIO3     9
#define INTERRUPT_RTC       10
#define INTERRUPT_SSP       11
#define INTERRUPT_UART0     12
#define INTERRUPT_UART1     13
#define INTERRUPT_UART2     14
#define INTERRUPT_SCI0      15
#define INTERRUPT_CLCD      16
#define INTERRUPT_DMA       17
#define INTERRUPT_PWRFAIL   18
#define INTERRUPT_MBX       19

static inline int interrupt_mask(int interrupt) {
  return 1 << (interrupt & 31);
}

#define SOFTWARE_INTERRUPT(x) ((*INT_SOFT) = interrupt_mask((x)))



#endif /* end of include guard: IRQ_H_QEMU */
