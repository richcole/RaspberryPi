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

.section .reset, "ax"
.global	__reset
.code 32

__reset:
	ldr	pc, reset_handler_address
	ldr	pc, undef_handler_address
	ldr	pc, swi_handler_address
	ldr	pc, prefetch_abort_handler_address
	ldr	pc, data_abort_handler_address
	b	.
	ldr	pc, irq_handler_address
	ldr	pc, fiq_handler_address

.asciz "(c) 2011 Simon Stapleton <simon.stapleton@gmail.com>"
.align
.asciz "Contains elements derived from the FreeBSD project (http://www.freebsd.org)"
.align
.asciz "In memory of John McCarthy, Sep 4, 1927 - Oct 24, 2011."
.align

reset_handler_address:		.word	_reset
undef_handler_address:		.word	__undef
swi_handler_address:		.word	_svc_handler
prefetch_abort_handler_address:	.word	__prefetch_abort
data_abort_handler_address:	.word	__data_abort
irq_handler_address:		.word	_irq_handler
fiq_handler_address:		.word	__fiq

/* Make globals to allow remappings */
.global reset_handler_address
.global	undef_handler_address
.global swi_handler_address
.global prefetch_abort_handler_address
.global data_abort_handler_address
.global irq_handler_address
.global fiq_handler_address

/* Weak for most of the default mappings */
.weak	__undef
.set	__undef, __no_handler
.weak	__prefetch_abort
.set	__prefetch_abort, __no_handler
.weak	__data_abort
.set	__data_abort, __no_handler
.weak	__fiq
.set	__fiq, __no_handler

/* Dummy handler that simply loops on itself */
FUNC	__no_handler
	b	__no_handler

