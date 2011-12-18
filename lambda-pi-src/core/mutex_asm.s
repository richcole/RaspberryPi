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

/* Try to acquire a mutex 			*/
/* returns 0 on success, 0xffffffff on failure 	*/
FUNC	mutex_acquire_nolock
	push	{r1-r3, lr}
	mov	r1, #0xff	/* "locked" */
.Ltry_again:
	ldrex	r2, [r0]	/* get the mutex itself */
	cmp	r1, r2		/* is it locked? */
	beq	.Llocked
	strex	r2, r1, [r0]	/* attempt to save */
	cmp	r2, #0x01	/* did the attempt succeed? */
	beq	.Ltry_again
	DMB	r2
.Llocked:
	movne	r0, #0x00	/* Success */
	moveq	r0, #0xffffffff
	pop	{r1-r3, pc}

FUNC	mutex_release
	push	{r0-r2, lr}
	mov	r1, #0x00	/* "unlocked" */
	DMB	r2
	str	r1, [r0]
	pop	{r0-r2, pc}
