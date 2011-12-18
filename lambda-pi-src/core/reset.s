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

.include "sysequates.inc"

 /*--- Start */

FUNC	_reset
	/* Do any hardware intialisation that absolutely must be done first */
	/* No stack set up at this point - be careful */
	ldr	r0, =.Lsize_memory
	ldr	r0, [r0]
	cmp	r0, #0
	blxne	r0

	/* Assume that at this point, __memtop and __system_ram are populated
	/* Let's get on with initialising our stacks */
	
	/* For the moment we'll work with the TNKernel/ARM assumption that */
	/* we only ever use SVC, IRQ and maybe FIQ */

	mrs	r0, cpsr			/* Original PSR value */
	ldr	r1, __memtop			/* Top of memory */

	bic	r0, r0, #MODE_BITS		/* Clear the mode bits */
	orr	r0, r0, #IRQ_MODE		/* Set IRQ mode bits */
	msr	cpsr_c, r0			/* Change the mode */
	mov	sp, r1				/* End of IRQ_STACK */
	
	/* Subtract IRQ stack size */
	ldr	r2, __irq_stack_size
	sbc	r1, r1, r2

	bic    r0, r0, #MODE_BITS		/* Clear the mode bits */
	orr    r0, r0, #SYS_MODE		/* Set SYS mode bits */
	msr    cpsr_c, r0			/* Change the mode   */
	mov    sp, r1				/* End of SYS_STACK  */
	
	/* Subtract SYS stack size */
	ldr	r2, __sys_stack_size
	sbc	r1, r1, r2

	bic    r0, r0, #MODE_BITS		/* Clear the mode bits */
	orr    r0, r0, #FIQ_MODE		/* Set FIQ mode bits */
	msr    cpsr_c, r0			/* Change the mode   */
	mov    sp, r1				/* End of FIQ_STACK  */
	
	/* Subtract FIQ stack size */
	ldr	r2, __fiq_stack_size
	sbc	r1, r1, r2

	bic    r0, r0, #MODE_BITS		/* Clear the mode bits */
	orr    r0, r0, #SVC_MODE		/* Set Supervisor mode bits */
	msr    cpsr_c, r0			/* Change the mode */
	mov    sp, r1				/* End of stack */
	
	/* And finally subtract Kernel stack size to get final __memtop */
	ldr	r2, __svc_stack_size
	sbc	r1, r1, r2
	str	r1, __memtop
	
	/*-- Leave core in SVC mode ! */
	
	/* Zero the memory in the .bss section.  */
	mov 	a2, #0			/* Second arg: fill value */
	mov	fp, a2			/* Null frame pointer */
	
	ldr	a1, .Lbss_start		/* First arg: start of memory block */
	ldr	a3, .Lbss_end	
	sub	a3, a3, a1		/* Third arg: length of block */
	bl	memset

	ldr r2, .Lc_entry		/* Let C coder have at initialisation */
        mov     lr, pc
        bx      r2

	cpsie	i			/* enable irq */
	cpsie	f			/* and fiq */

	/* Initialisation done, sleep */
	ldr r2, .Lsleep
        mov     lr, pc
        bx      r2

	
/* Variables (hopefully) provided by the linker */

.Lbss_start:		.word	__bss_start__
.Lbss_end:		.word	__bss_end__
.Lc_entry:		.word	c_entry
.Lsleep:		.word	sys_sleep

/* Defaulted variables */
.Lsize_memory:		.word	_size_memory
.weak	_size_memory

/* These ones are exposed to C */
.global	__memtop
__memtop:		.word	0x00400000		/* Start checking memory from 4MB */
.global	__system_ram
__system_ram:		.word	0x00000000		/* System memory in MB */
.global	__heap_start
__heap_start:		.word	__bss_end__		/* Start of the dynamic heap */
.global __heap_top
__heap_top:		.word	__bss_end__		/* Current end of dynamic heap */

/* These ones are global but not exposed in header files */
.global	__mem_page_size
__mem_page_size:	.word	0x00100000		/* Scan 1MB blocks */
.global __irq_stack_size
__irq_stack_size:	.word	0x00000100		/* Stack size for IRQMODE in bytes */
.global __irq_stack_size
__sys_stack_size:	.word	0x00000100		/* Stack size for SYSMODE in bytes */
.global __sys_stack_size
__fiq_stack_size:	.word	0x00000100		/* Stack size for FIQMODE in bytes */
.global __svc_stack_size
__svc_stack_size:	.word	0x00008000		/* Stack size for SVCMODE in bytes */

