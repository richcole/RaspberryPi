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

/* SVC call handler */
/* called using svc #syscall_number with arguments in r0-r4 */
/* Same preamble as an interrupt, we exit the same way */
FUNC	_svc_handler
	srsdb	sp!, #SVC_MODE	/* save LR_svc and SPSR_svc to svc mode stack */
	push	{r0-r12}	/* Save registers */

	and	r0, sp, #4	/* align the stack and save adjustment with LR_user */
	sub	sp, sp, r0
	push	{r0, lr}
		
	ldr	r0,[lr,#-4]		/* Calculate address of SVC instruction */
					/* and load it into R0. */
	and	r0,r0,#0x000000ff	/* Mask off top 24 bits of instruction */
					/* to give SVC number. */
					
	ldr	r2, =__syscall_table	/* get the syscall */
	ldr	r3, [r2, r0, lsl#2]
	cmp	r3, #0
	beq	_syscall_exit
	tst	r3, #0x01		/* what linkage are we using */
	bxeq	r3			/* ASM, just run away */
	bic	r3, r3, #0x01
	blx	r3			/* C, must come back here */

.global _syscall_exit
_syscall_exit:
	pop	{r0, lr}		/* restore LR_user and readjust stack */
	add	sp, sp, r0

	pop	{r0-r12}		/* and other registers */
	rfeia	sp!			/* before returning */

.section .bss
.global __syscall_table
__syscall_table:
	/* leave space for 256 syscall addresses */
	.skip	2048
@	.space	2048
	