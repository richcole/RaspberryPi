#ifndef SYSCALLS_H
#define SYSCALLS_H
/*	  Copyright (c) 20011, Simon Stapleton (simon.stapleton@gmail.com)	  */
/*										  */
/*				All rights reserved.				  */
/*										  */
/* Redistribution  and	use   in  source  and  binary  forms,	with  or  without */
/* modification, are permitted provided that the following conditions are met:	  */
/*										  */
/* Redistributions of  source code must	 retain the above copyright  notice, this */
/* list of conditions and the following disclaimer.				  */
/*										  */
/* Redistributions in binary form must reproduce the above copyright notice, this */
/* list of conditions and the following disclaimer in the documentation and/or	  */
/* other materials provided with the distribution.				  */
/*										  */
/* Neither the name of	the developer nor the names of	other contributors may be */
/* used	 to  endorse or	 promote  products  derived  from this	software  without */
/* specific prior written permission.						  */
/*										  */
/* THIS SOFTWARE  IS PROVIDED BY THE  COPYRIGHT HOLDERS AND CONTRIBUTORS  "AS IS" */
/* AND ANY  EXPRESS OR	IMPLIED WARRANTIES,  INCLUDING, BUT  NOT LIMITED  TO, THE */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE */
/* DISCLAIMED. IN NO  EVENT SHALL THE COPYRIGHT HOLDER OR  CONTRIBUTORS BE LIABLE */
/* FOR	ANY DIRECT,  INDIRECT, INCIDENTAL,  SPECIAL, EXEMPLARY,	 OR CONSEQUENTIAL */
/* DAMAGES (INCLUDING,	BUT NOT	 LIMITED TO, PROCUREMENT  OF SUBSTITUTE	 GOODS OR */
/* SERVICES; LOSS  OF USE,  DATA, OR PROFITS;  OR BUSINESS  INTERRUPTION) HOWEVER */
/* CAUSED AND ON ANY THEORY OF	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING	IN ANY WAY OUT OF THE USE */
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.		  */

#include "bsp.h"

typedef enum {
  NAKED = 0,
  C_LINKAGE = 1,
} syscall_linkage_t;

typedef void(*syscall_handler_t)(uint32_t, uint32_t *);

void init_syscalls(void);
void define_syscall(syscall_linkage_t linkage, uint32_t number, void * func);

#define MAX_SYSCALLS    0xff

// Actual syscalls
#define SYS_YIELD       0

// Non-returning syscalls
#define sc0_nr(number) __asm__ volatile ("svc #" #number "\n\t")
#define sc1_nr(number, arg1) \
  __asm__ volatile (              \
	"mov	r0, %[a1]\n\t"               \
  "push {r4, lr}\n\t" \
	"svc #" #number "\n\t"     \
	"pop {r4, lr}\n\t" \
  :: [a1] "r" (arg1))

#define sc2_nr(number, arg1, arg2) \
  __asm__ volatile (              \
  "mov	r0, %[a1]\n\t"               \
  "mov	r1, %[a2]\n\t"               \
  "push {r4, lr}\n\t" \
	"svc #" #number "\n\t"     \
	"pop {r4, lr}\n\t" \
  :: [a1] "r" (arg1), [a2] "r" (arg2))

#define sc3_nr(number, arg1, arg2, arg3) \
  __asm__ volatile (              \
	"mov	r0, %[a1]\n\t"               \
	"mov	r1, %[a2]\n\t"               \
	"mov	r2, %[a3]\n\t"               \
  "push {r4, lr}\n\t" \
	"svc #" #number "\n\t"     \
	"pop {r4, lr}\n\t" \
  :: [a1] "r" (arg1), [a2] "r" (arg2), [a3] "r" (arg3))

#define sc4_nr(number, arg1, arg2, arg3, arg4) \
  __asm__ volatile (              \
	"mov	r0, %[a1]\n\t"               \
	"mov	r1, %[a2]\n\t"               \
	"mov	r2, %[a3]\n\t"               \
	"mov	r3, %[a4]\n\t"               \
  "push {r4, lr}\n\t" \
	"svc #" #number "\n\t"     \
	"pop {r4, lr}\n\t" \
  :: [a1] "r" (arg1), [a2] "r" (arg2), [a3] "r" (arg3), [a4] "r" (arg4))

// Returning syscalls
#define sc1_r(number, arg1, result) \
  __asm__ volatile (              \
	"mov	r0, %[a1]\n\t"               \
  "push {r4, lr}\n\t" \
	"svc #" #number "\n\t"     \
	"pop {r4, lr}\n\t" \
	"mov  %[r], r0\n\t"               \
  : [r] "=r" (result) : [a1] "r" (arg1))

#define sc2_r(number, arg1, arg2, result) \
  __asm__ volatile (              \
  "mov	r0, %[a1]\n\t"               \
  "mov	r1, %[a2]\n\t"               \
  "push {r4, lr}\n\t" \
	"svc #" #number "\n\t"     \
	"pop {r4, lr}\n\t" \
	"mov  %[r], r0\n\t"               \
  : [r] "=r" (result) : [a1] "r" (arg1), [a2] "r" (arg2))

#define sc3_r(number, arg1, arg2, arg3, result) \
  __asm__ volatile (              \
	"mov	r0, %[a1]\n\t"               \
	"mov	r1, %[a2]\n\t"               \
	"mov	r2, %[a3]\n\t"               \
  "push {r4, lr}\n\t" \
	"svc #" #number "\n\t"     \
	"pop {r4, lr}\n\t" \
	"mov  %[r], r0\n\t"               \
  : [r] "=r" (result) : [a1] "r" (arg1), [a2] "r" (arg2), [a3] "r" (arg3))

#define sc4_r(number, arg1, arg2, arg3, arg4, result) \
  __asm__ volatile (              \
	"mov	r0, %[a1]\n\t"               \
	"mov	r1, %[a2]\n\t"               \
	"mov	r2, %[a3]\n\t"               \
	"mov	r3, %[a3]\n\t"               \
  "push {r4, lr}\n\t" \
	"svc #" #number "\n\t"     \
	"pop {r4, lr}\n\t" \
	"mov  %[r], r0\n\t"               \
  : [r] "=r" (result) : [a1] "r" (arg1), [a2] "r" (arg2), [a3] "r" (arg3), [a4] "r" (arg4))

// Bloody preprocessor mess
#define syscall0_nr(number) sc0_nr(number)
#define syscall1_nr(number, arg1) sc1_nr(number, arg1)
#define syscall2_nr(number, arg1, arg2) sc2_nr(number, arg1, arg2)
#define syscall3_nr(number, arg1, arg2, arg3) sc3_nr(number, arg1, arg2, arg3)
#define syscall4_nr(number, arg1, arg2, arg3, arg4) sc4_nr(number, arg1, arg2, arg3, arg4)
// Returning syscalls
#define syscall1_r(number, arg1, result) sc1_r(number, arg1, result)
#define syscall2_r(number, arg1, arg2, result) sc2_r(number, arg1, arg2, result)
#define syscall3_r(number, arg1, arg2, arg3, result) sc3_r(number, arg1, arg2, arg3, result)
#define syscall4_r(number, arg1, arg2, arg3, arg4, result) sc4_r(number, arg1, arg2, arg3, arg4, result)

void yield();

#endif /* end of include guard: SYSCALLS_H */
