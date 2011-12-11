#ifndef SYSMACROS_H
#define SYSMACROS_H
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

#include "stdint.h"

#define read32(addr) (*(uint32_t*)(addr))
#define read16(addr) (*(uint16_t*)(addr))
#define read8(addr) (*(uint8_t*)(addr))

#define write32(addr,value) (*(uint32_t*)(addr) = (uint32_t)(value))
#define write16(addr,value) (*(uint16_t*)(addr) = (uint16_t)(value))
#define write8(addr,value) (*(uint8_t*)(addr) = (uint8_t)(value))

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )
#define DMB_IMPL(name) { uint32_t name = 0; __asm__ __volatile__ ("mcr	p15,0,%[t],c7,c10,5\n" :: [t] "r" (name) : "memory"); }
#define DSB_IMPL(name) { uint32_t name = 0; __asm__ __volatile__ ("mcr	p15,0,%[t],c7,c10,4\n" :: [t] "r" (name) : "memory"); }

#define DMB DMB_IMPL(MACRO_CONCAT(__dmb_, __COUNTER__))
#define DSB DSB_IMPL(MACRO_CONCAT(__dsb_, __COUNTER__))

/* Stuff for the GP15 control Register */
#define CP15_CR_M_BIT		1 << 0
#define CP15_CR_A_BIT		1 << 1
#define CP15_CR_C_BIT		1 << 2
#define CP15_CR_W_BIT		1 << 3
#define CP15_CR_B_BIT		1 << 7
#define CP15_CR_S_BIT		1 << 8
#define CP15_CR_R_BIT		1 << 9
#define CP15_CR_F_BIT		1 << 10
#define CP15_CR_Z_BIT		1 << 11
#define CP15_CR_I_BIT		1 << 12
#define CP15_CR_V_BIT		1 << 13
#define CP15_CR_RR_BIT	1 << 14
#define CP15_CR_L4_BIT	1 << 15
#define CP15_CR_DT_BIT	1 << 16
#define CP15_CR_IT_BIT	1 << 18
#define CP15_CR_FI_BIT	1 << 21
#define CP15_CR_U_BIT		1 << 22
#define CP15_CR_XP_BIT	1 << 23
#define CP15_CR_VE_BIT	1 << 24
#define CP15_CR_EE_BIT	1 << 25
#define CP15_CR_TR_BIT	1 << 28
#define CP15_CR_FA_BIT	1 << 29


#define SET_CP15_CR_IMPL(bits, name) \
uint32_t name = 0; \
__asm__ __volatile__ ("mrc	p15, 0, %[t], c1, c0, 0" : [t] "=r" (name)); \
name = name | (uint32_t)(bits); \
__asm__ __volatile__ ("mrc	p15, 0, %[t], c1, c0, 0" :: [t] "r" (name));
#define SET_CP15_CR(bits) SET_CP15_CR_IMPL(bits, MACRO_CONCAT(__cp15_cr_, __COUNTER__))

#define CLEAR_CP15_CR_IMPL(bits, name) \
uint32_t name = 0; \
__asm__ __volatile__ ("mrc	p15, 0, %[t], c1, c0, 0" : [t] "=r" (name)); \
name = name & !(uint32_t)(bits); \
__asm__ __volatile__ ("mrc	p15, 0, %[t], c1, c0, 0" :: [t] "r" (name));
#define CLEAR_CP15_CR(bits) CLEAR_CP15_CR_IMPL(bits, MACRO_CONCAT(__cp15_cr_, __COUNTER__))

#define WFI_IMPL(name) \
uint32_t name = 0; \
__asm__ __volatile__ ("MCR p15,0,%[t],c7,c0,4" :: [t] "r" (name));
#define WFI WFI_IMPL(MACRO_CONCAT(__wfi_,__COUNTER__))

#define INTERRUPTS_OFF_PRIV __asm__ __volatile__("cpsid	i")
#define INTERRUPTS_ON_PRIV __asm__ __volatile__("cpsie	i")

#define REG32(x)  (uint32_t*)((uint32_t)(x))
#define REG16(x)  (uint16_t*)((uint32_t)(x))

#endif /* end of include guard: SYSMACROS_H */
