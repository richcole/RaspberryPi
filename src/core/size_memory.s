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

/* This tries to work out how much memory we have available	*/
/* Should work on both Pi and qemu targets			*/
FUNC	_size_memory

	/* patch in temporary fault handler */
	ldr	r5, =.Ldaha
	ldr	r5, [r5]
	ldr	r6, [r5]
	ldr	r7, =temp_abort_handler
	str	r7, [r5] 
	DMB	r12

	/* Try and work out how much memory we have */
	ldr	r0, .Lmemtop
	ldr	r1, .Lmem_page_size
	ldr	r1, [r1]
	ldr	r2, .Lsystem_ram
	ldr	r3, [r0]
.Lmem_check:
	add	r3, r3, #0x04
	str	r3, [r3]		/* Try and store a value above current __memtop */
	DMB	r12			/* Data memory barrier, in case */
	ldr	r4, [r3]		/* Test if it stored */
	cmp	r3, r4			/* Did it work? */
	bne	.Lmem_done
	ldr	r3, [r0]
	add	r3, r3, r1		/* Add block size onto __memtop and try again */	
	str	r3, [r0]
	b	.Lmem_check
.Lmem_done:
	ldr	r3, [r0]		/* get final memory size */
	lsr	r3, #0x14		/* Get number of megabytes */
	str	r3, [r2]		/* And store it */
	
	/* unpatch handlers */
	str	r6, [r5]
	DMB	r12

	bx	lr
.Ldaha:
.extern	data_abort_handler_address
.word	data_abort_handler_address

.Lmemtop:
.extern __memtop
.word	__memtop

.Lmem_page_size:
.extern __mem_page_size
.word __mem_page_size

.Lsystem_ram:
.extern __system_ram
.word __system_ram


/* temporary data abort handler that sets r4 to zero */
/* this will force the "normal" check to work in the */
/* case (as, I believe, on RasPi) where access 'out  */
/* of bounds' causes a page fault                    */

temp_abort_handler:
	mov	r4, #0x00000000
	sub	lr, lr, #0x08
	movs	pc, lr
