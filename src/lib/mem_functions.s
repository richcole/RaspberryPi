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

/* Adapted & brutalised from FreeBSD 10 support.S */
.include "macros.inc"

.L_min_memcpy_size:
.word   _min_memcpy_size
.weak	_min_memcpy_size
.L_min_bzero_size:
.word   _min_bzero_size
.weak	_min_bzero_size


/*
 * memset: Sets a block of memory to the specified value
 *
 * On entry:
 *   r0 - dest address
 *   r1 - byte to write
 *   r2 - number of bytes to write
 *
 * On exit:
 *   r0 - dest address
 */
/* LINTSTUB: Func: void bzero(void *, size_t) */
FUNC bzero
.Lnormal0:
	mov	r3, #0x00
	b	do_memset

/* LINTSTUB: Func: void *memset(void *, int, size_t) */
FUNC memset
	and	r3, r1, #0xff		/* We deal with bytes */
	mov	r1, r2
do_memset:
	cmp	r1, #0x04		/* Do we have less than 4 bytes */
	mov	ip, r0
	blt	.Lmemset_lessthanfour

	/* Ok first we will word align the address */
	ands	r2, ip, #0x03		/* Get the bottom two bits */
	bne	.Lmemset_wordunaligned	/* The address is not word aligned */

	/* We are now word aligned */
.Lmemset_wordaligned:
	orr	r3, r3, r3, lsl #8	/* Extend value to 16-bits */
	cmp	r1, #0x10
	orr	r3, r3, r3, lsl #16	/* Extend value to 32-bits */
	blt	.Lmemset_loop4		/* If less than 16 then use words */
	mov	r2, r3			/* Duplicate data */
	cmp	r1, #0x80		/* If < 128 then skip the big loop */
	blt	.Lmemset_loop32

	/* Do 128 bytes at a time */
.Lmemset_loop128:
	subs	r1, r1, #0x80
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	bgt	.Lmemset_loop128
	moveq   pc, lr			/* Zero length so just exit */

	add	r1, r1, #0x80		/* Adjust for extra sub */

	/* Do 32 bytes at a time */
.Lmemset_loop32:
	subs	r1, r1, #0x20
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	bgt	.Lmemset_loop32
	moveq   pc, lr			/* Zero length so just exit */

	adds	r1, r1, #0x10		/* Partially adjust for extra sub */

	/* Deal with 16 bytes or more */
	stmgeia	ip!, {r2-r3}
	stmgeia	ip!, {r2-r3}
	moveq   pc, lr			/* Zero length so just exit */

	addlt	r1, r1, #0x10		/* Possibly adjust for extra sub */

	/* We have at least 4 bytes so copy as words */
.Lmemset_loop4:
	subs	r1, r1, #0x04
	strge	r3, [ip], #0x04
	bgt	.Lmemset_loop4
	moveq   pc, lr			/* Zero length so just exit */

	cmp	r1, #-2

	strb	r3, [ip], #0x01		/* Set 1 byte */
	strgeb	r3, [ip], #0x01		/* Set another byte */
	strgtb	r3, [ip]		/* and a third */
	mov     pc, lr			/* Exit */

.Lmemset_wordunaligned:
	rsb	r2, r2, #0x004
	strb	r3, [ip], #0x01		/* Set 1 byte */
	cmp	r2, #0x02
	strgeb	r3, [ip], #0x01		/* Set another byte */
	sub	r1, r1, r2
	strgtb	r3, [ip], #0x01		/* and a third */
	cmp	r1, #0x04		/* More than 4 bytes left? */
	bge	.Lmemset_wordaligned	/* Yup */

.Lmemset_lessthanfour:
	cmp	r1, #0x00
	moveq   pc, lr			/* Zero length so exit */
	strb	r3, [ip], #0x01		/* Set 1 byte */
	cmp	r1, #0x02
	strgeb	r3, [ip], #0x01		/* Set another byte */
	strgtb	r3, [ip]		/* and a third */
	mov     pc, lr			/* Exit */

FUNC bcmp
.global memcmp
memcmp:
	mov	ip, r0
	cmp	r2, #0x06
	beq	.Lmemcmp_6bytes
	mov	r0, #0x00

	/* Are both addresses aligned the same way? */
	cmp	r2, #0x00
	eornes	r3, ip, r1
	moveq   pc, lr			/* len == 0, or same addresses! */
	tst	r3, #0x03
	subne	r2, r2, #0x01
	bne	.Lmemcmp_bytewise2	/* Badly aligned. Do it the slow way */

	/* Word-align the addresses, if necessary */
	sub	r3, r1, #0x05
	ands	r3, r3, #0x03
	add	r3, r3, r3, lsl #1
	addne	pc, pc, r3, lsl #3
	nop

	/* Compare up to 3 bytes */
	ldrb	r0, [ip], #0x01
	ldrb	r3, [r1], #0x01
	subs	r0, r0, r3
	movne   pc, lr
	subs	r2, r2, #0x01
	moveq   pc, lr

	/* Compare up to 2 bytes */
	ldrb	r0, [ip], #0x01
	ldrb	r3, [r1], #0x01
	subs	r0, r0, r3
	movne   pc, lr
	subs	r2, r2, #0x01
	moveq   pc, lr

	/* Compare 1 byte */
	ldrb	r0, [ip], #0x01
	ldrb	r3, [r1], #0x01
	subs	r0, r0, r3
	movne   pc, lr
	subs	r2, r2, #0x01
	moveq   pc, lr

	/* Compare 4 bytes at a time, if possible */
	subs	r2, r2, #0x04
	bcc	.Lmemcmp_bytewise
.Lmemcmp_word_aligned:
	ldr	r0, [ip], #0x04
	ldr	r3, [r1], #0x04
	subs	r2, r2, #0x04
	cmpcs	r0, r3
	beq	.Lmemcmp_word_aligned
	sub	r0, r0, r3

	/* Correct for extra subtraction, and check if done */
	adds	r2, r2, #0x04
	cmpeq	r0, #0x00		/* If done, did all bytes match? */
	moveq   pc, lr			/* Yup. Just return */

	/* Re-do the final word byte-wise */
	sub	ip, ip, #0x04
	sub	r1, r1, #0x04

.Lmemcmp_bytewise:
	add	r2, r2, #0x03
.Lmemcmp_bytewise2:
	ldrb	r0, [ip], #0x01
	ldrb	r3, [r1], #0x01
	subs	r2, r2, #0x01
	cmpcs	r0, r3
	beq	.Lmemcmp_bytewise2
	sub	r0, r0, r3
	mov     pc, lr

	/*
	 * 6 byte compares are very common, thanks to the network stack.
	 * This code is hand-scheduled to reduce the number of stalls for
	 * load results. Everything else being equal, this will be ~32%
	 * faster than a byte-wise memcmp.
	 */
	.align	5
.Lmemcmp_6bytes:
	ldrb	r3, [r1, #0x00]		/* r3 = b2#0 */
	ldrb	r0, [ip, #0x00]		/* r0 = b1#0 */
	ldrb	r2, [r1, #0x01]		/* r2 = b2#1 */
	subs	r0, r0, r3		/* r0 = b1#0 - b2#0 */
	ldreqb	r3, [ip, #0x01]		/* r3 = b1#1 */
	movne   pc, lr			/* Return if mismatch on #0 */
	subs	r0, r3, r2		/* r0 = b1#1 - b2#1 */
	ldreqb	r3, [r1, #0x02]		/* r3 = b2#2 */
	ldreqb	r0, [ip, #0x02]		/* r0 = b1#2 */
	movne   pc, lr			/* Return if mismatch on #1 */
	ldrb	r2, [r1, #0x03]		/* r2 = b2#3 */
	subs	r0, r0, r3		/* r0 = b1#2 - b2#2 */
	ldreqb	r3, [ip, #0x03]		/* r3 = b1#3 */
	movne   pc, lr			/* Return if mismatch on #2 */
	subs	r0, r3, r2		/* r0 = b1#3 - b2#3 */
	ldreqb	r3, [r1, #0x04]		/* r3 = b2#4 */
	ldreqb	r0, [ip, #0x04]		/* r0 = b1#4 */
	movne   pc, lr			/* Return if mismatch on #3 */
	ldrb	r2, [r1, #0x05]		/* r2 = b2#5 */
	subs	r0, r0, r3		/* r0 = b1#4 - b2#4 */
	ldreqb	r3, [ip, #0x05]		/* r3 = b1#5 */
	movne   pc, lr			/* Return if mismatch on #4 */
	sub	r0, r3, r2		/* r0 = b1#5 - b2#5 */
	mov     pc, lr

FUNC bcopy
	/* switch the source and destination registers */
	eor     r0, r1, r0 
	eor     r1, r0, r1 
	eor     r0, r1, r0 
FUNC memmove
	/* Do the buffers overlap? */
	cmp	r0, r1
	moveq   pc, lr		/* Bail now if src/dst are the same */
	subcc	r3, r0, r1	/* if (dst > src) r3 = dst - src */
	subcs	r3, r1, r0	/* if (src > dsr) r3 = src - dst */
	cmp	r3, r2		/* if (r3 < len) we have an overlap */
	bcc	memcpy

	/* Determine copy direction */
	cmp	r1, r0
	bcc	.Lmemmove_backwards

	moveq	r0, #0			/* Quick abort for len=0 */
	moveq   pc, lr

	stmdb	sp!, {r0, lr}		/* memmove() returns dest addr */
	subs	r2, r2, #4
	blt	.Lmemmove_fl4		/* less than 4 bytes */
	ands	r12, r0, #3
	bne	.Lmemmove_fdestul	/* oh unaligned destination addr */
	ands	r12, r1, #3
	bne	.Lmemmove_fsrcul		/* oh unaligned source addr */

.Lmemmove_ft8:
	/* We have aligned source and destination */
	subs	r2, r2, #8
	blt	.Lmemmove_fl12		/* less than 12 bytes (4 from above) */
	subs	r2, r2, #0x14         
	blt	.Lmemmove_fl32		/* less than 32 bytes (12 from above) */
	stmdb	sp!, {r4}		/* borrow r4 */

	/* blat 32 bytes at a time */
	/* XXX for really big copies perhaps we should use more registers */
.Lmemmove_floop32:	
	ldmia	r1!, {r3, r4, r12, lr}
	stmia	r0!, {r3, r4, r12, lr}
	ldmia	r1!, {r3, r4, r12, lr}
	stmia	r0!, {r3, r4, r12, lr}
	subs	r2, r2, #0x20         
	bge	.Lmemmove_floop32

	cmn	r2, #0x10
	ldmgeia	r1!, {r3, r4, r12, lr}	/* blat a remaining 16 bytes */
	stmgeia	r0!, {r3, r4, r12, lr}
	subge	r2, r2, #0x10         
	ldmia	sp!, {r4}		/* return r4 */

.Lmemmove_fl32:
	adds	r2, r2, #0x14         

	/* blat 12 bytes at a time */
.Lmemmove_floop12:
	ldmgeia	r1!, {r3, r12, lr}
	stmgeia	r0!, {r3, r12, lr}
	subges	r2, r2, #0x0c         
	bge	.Lmemmove_floop12

.Lmemmove_fl12:
	adds	r2, r2, #8
	blt	.Lmemmove_fl4

	subs	r2, r2, #4
	ldrlt	r3, [r1], #4
	strlt	r3, [r0], #4
	ldmgeia	r1!, {r3, r12}
	stmgeia	r0!, {r3, r12}
	subge	r2, r2, #4

.Lmemmove_fl4:
	/* less than 4 bytes to go */
	adds	r2, r2, #4
	ldmeqia	sp!, {r0, pc}		/* done */

	/* copy the crud byte at a time */
	cmp	r2, #2
	ldrb	r3, [r1], #1
	strb	r3, [r0], #1
	ldrgeb	r3, [r1], #1
	strgeb	r3, [r0], #1
	ldrgtb	r3, [r1], #1
	strgtb	r3, [r0], #1
	ldmia	sp!, {r0, pc}

	/* erg - unaligned destination */
.Lmemmove_fdestul:
	rsb	r12, r12, #4
	cmp	r12, #2

	/* align destination with byte copies */
	ldrb	r3, [r1], #1
	strb	r3, [r0], #1
	ldrgeb	r3, [r1], #1
	strgeb	r3, [r0], #1
	ldrgtb	r3, [r1], #1
	strgtb	r3, [r0], #1
	subs	r2, r2, r12
	blt	.Lmemmove_fl4		/* less the 4 bytes */

	ands	r12, r1, #3
	beq	.Lmemmove_ft8		/* we have an aligned source */

	/* erg - unaligned source */
	/* This is where it gets nasty ... */
.Lmemmove_fsrcul:
	bic	r1, r1, #3
	ldr	lr, [r1], #4
	cmp	r12, #2
	bgt	.Lmemmove_fsrcul3
	beq	.Lmemmove_fsrcul2
	cmp	r2, #0x0c            
	blt	.Lmemmove_fsrcul1loop4
	sub	r2, r2, #0x0c         
	stmdb	sp!, {r4, r5}

.Lmemmove_fsrcul1loop16:
	mov	r3, lr, lsr #8
	ldmia	r1!, {r4, r5, r12, lr}
	orr	r3, r3, r4, lsl #24
	mov	r4, r4, lsr #8
	orr	r4, r4, r5, lsl #24
	mov	r5, r5, lsr #8
	orr	r5, r5, r12, lsl #24
	mov	r12, r12, lsr #8
	orr	r12, r12, lr, lsl #24
	stmia	r0!, {r3-r5, r12}
	subs	r2, r2, #0x10         
	bge	.Lmemmove_fsrcul1loop16
	ldmia	sp!, {r4, r5}
	adds	r2, r2, #0x0c         
	blt	.Lmemmove_fsrcul1l4

.Lmemmove_fsrcul1loop4:
	mov	r12, lr, lsr #8
	ldr	lr, [r1], #4
	orr	r12, r12, lr, lsl #24
	str	r12, [r0], #4
	subs	r2, r2, #4
	bge	.Lmemmove_fsrcul1loop4

.Lmemmove_fsrcul1l4:
	sub	r1, r1, #3
	b	.Lmemmove_fl4

.Lmemmove_fsrcul2:
	cmp	r2, #0x0c            
	blt	.Lmemmove_fsrcul2loop4
	sub	r2, r2, #0x0c         
	stmdb	sp!, {r4, r5}

.Lmemmove_fsrcul2loop16:
	mov	r3, lr, lsr #16
	ldmia	r1!, {r4, r5, r12, lr}
	orr	r3, r3, r4, lsl #16
	mov	r4, r4, lsr #16
	orr	r4, r4, r5, lsl #16
	mov	r5, r5, lsr #16
	orr	r5, r5, r12, lsl #16
	mov	r12, r12, lsr #16
	orr	r12, r12, lr, lsl #16
	stmia	r0!, {r3-r5, r12}
	subs	r2, r2, #0x10         
	bge	.Lmemmove_fsrcul2loop16
	ldmia	sp!, {r4, r5}
	adds	r2, r2, #0x0c         
	blt	.Lmemmove_fsrcul2l4

.Lmemmove_fsrcul2loop4:
	mov	r12, lr, lsr #16
	ldr	lr, [r1], #4
	orr	r12, r12, lr, lsl #16
	str	r12, [r0], #4
	subs	r2, r2, #4
	bge	.Lmemmove_fsrcul2loop4

.Lmemmove_fsrcul2l4:
	sub	r1, r1, #2
	b	.Lmemmove_fl4

.Lmemmove_fsrcul3:
	cmp	r2, #0x0c            
	blt	.Lmemmove_fsrcul3loop4
	sub	r2, r2, #0x0c         
	stmdb	sp!, {r4, r5}

.Lmemmove_fsrcul3loop16:
	mov	r3, lr, lsr #24
	ldmia	r1!, {r4, r5, r12, lr}
	orr	r3, r3, r4, lsl #8
	mov	r4, r4, lsr #24
	orr	r4, r4, r5, lsl #8
	mov	r5, r5, lsr #24
	orr	r5, r5, r12, lsl #8
	mov	r12, r12, lsr #24
	orr	r12, r12, lr, lsl #8
	stmia	r0!, {r3-r5, r12}
	subs	r2, r2, #0x10         
	bge	.Lmemmove_fsrcul3loop16
	ldmia	sp!, {r4, r5}
	adds	r2, r2, #0x0c         
	blt	.Lmemmove_fsrcul3l4

.Lmemmove_fsrcul3loop4:
	mov	r12, lr, lsr #24
	ldr	lr, [r1], #4
	orr	r12, r12, lr, lsl #8
	str	r12, [r0], #4
	subs	r2, r2, #4
	bge	.Lmemmove_fsrcul3loop4

.Lmemmove_fsrcul3l4:
	sub	r1, r1, #1
	b	.Lmemmove_fl4

.Lmemmove_backwards:
	add	r1, r1, r2
	add	r0, r0, r2
	subs	r2, r2, #4
	blt	.Lmemmove_bl4		/* less than 4 bytes */
	ands	r12, r0, #3
	bne	.Lmemmove_bdestul	/* oh unaligned destination addr */
	ands	r12, r1, #3
	bne	.Lmemmove_bsrcul		/* oh unaligned source addr */

.Lmemmove_bt8:
	/* We have aligned source and destination */
	subs	r2, r2, #8
	blt	.Lmemmove_bl12		/* less than 12 bytes (4 from above) */
	stmdb	sp!, {r4, lr}
	subs	r2, r2, #0x14		/* less than 32 bytes (12 from above) */
	blt	.Lmemmove_bl32

	/* blat 32 bytes at a time */
	/* XXX for really big copies perhaps we should use more registers */
.Lmemmove_bloop32:
	ldmdb	r1!, {r3, r4, r12, lr}
	stmdb	r0!, {r3, r4, r12, lr}
	ldmdb	r1!, {r3, r4, r12, lr}
	stmdb	r0!, {r3, r4, r12, lr}
	subs	r2, r2, #0x20         
	bge	.Lmemmove_bloop32

.Lmemmove_bl32:
	cmn	r2, #0x10            
	ldmgedb	r1!, {r3, r4, r12, lr}	/* blat a remaining 16 bytes */
	stmgedb	r0!, {r3, r4, r12, lr}
	subge	r2, r2, #0x10         
	adds	r2, r2, #0x14         
	ldmgedb	r1!, {r3, r12, lr}	/* blat a remaining 12 bytes */
	stmgedb	r0!, {r3, r12, lr}
	subge	r2, r2, #0x0c         
	ldmia	sp!, {r4, lr}

.Lmemmove_bl12:
	adds	r2, r2, #8
	blt	.Lmemmove_bl4
	subs	r2, r2, #4
	ldrlt	r3, [r1, #-4]!
	strlt	r3, [r0, #-4]!
	ldmgedb	r1!, {r3, r12}
	stmgedb	r0!, {r3, r12}
	subge	r2, r2, #4

.Lmemmove_bl4:
	/* less than 4 bytes to go */
	adds	r2, r2, #4
	moveq   pc, lr			/* done */

	/* copy the crud byte at a time */
	cmp	r2, #2
	ldrb	r3, [r1, #-1]!
	strb	r3, [r0, #-1]!
	ldrgeb	r3, [r1, #-1]!
	strgeb	r3, [r0, #-1]!
	ldrgtb	r3, [r1, #-1]!
	strgtb	r3, [r0, #-1]!
	mov     pc, lr

	/* erg - unaligned destination */
.Lmemmove_bdestul:
	cmp	r12, #2

	/* align destination with byte copies */
	ldrb	r3, [r1, #-1]!
	strb	r3, [r0, #-1]!
	ldrgeb	r3, [r1, #-1]!
	strgeb	r3, [r0, #-1]!
	ldrgtb	r3, [r1, #-1]!
	strgtb	r3, [r0, #-1]!
	subs	r2, r2, r12
	blt	.Lmemmove_bl4		/* less than 4 bytes to go */
	ands	r12, r1, #3
	beq	.Lmemmove_bt8		/* we have an aligned source */

	/* erg - unaligned source */
	/* This is where it gets nasty ... */
.Lmemmove_bsrcul:
	bic	r1, r1, #3
	ldr	r3, [r1, #0]
	cmp	r12, #2
	blt	.Lmemmove_bsrcul1
	beq	.Lmemmove_bsrcul2
	cmp	r2, #0x0c            
	blt	.Lmemmove_bsrcul3loop4
	sub	r2, r2, #0x0c         
	stmdb	sp!, {r4, r5, lr}

.Lmemmove_bsrcul3loop16:
	mov	lr, r3, lsl #8
	ldmdb	r1!, {r3-r5, r12}
	orr	lr, lr, r12, lsr #24
	mov	r12, r12, lsl #8
	orr	r12, r12, r5, lsr #24
	mov	r5, r5, lsl #8
	orr	r5, r5, r4, lsr #24
	mov	r4, r4, lsl #8
	orr	r4, r4, r3, lsr #24
	stmdb	r0!, {r4, r5, r12, lr}
	subs	r2, r2, #0x10         
	bge	.Lmemmove_bsrcul3loop16
	ldmia	sp!, {r4, r5, lr}
	adds	r2, r2, #0x0c         
	blt	.Lmemmove_bsrcul3l4

.Lmemmove_bsrcul3loop4:
	mov	r12, r3, lsl #8
	ldr	r3, [r1, #-4]!
	orr	r12, r12, r3, lsr #24
	str	r12, [r0, #-4]!
	subs	r2, r2, #4
	bge	.Lmemmove_bsrcul3loop4

.Lmemmove_bsrcul3l4:
	add	r1, r1, #3
	b	.Lmemmove_bl4

.Lmemmove_bsrcul2:
	cmp	r2, #0x0c            
	blt	.Lmemmove_bsrcul2loop4
	sub	r2, r2, #0x0c         
	stmdb	sp!, {r4, r5, lr}

.Lmemmove_bsrcul2loop16:
	mov	lr, r3, lsl #16
	ldmdb	r1!, {r3-r5, r12}
	orr	lr, lr, r12, lsr #16
	mov	r12, r12, lsl #16
	orr	r12, r12, r5, lsr #16
	mov	r5, r5, lsl #16
	orr	r5, r5, r4, lsr #16
	mov	r4, r4, lsl #16
	orr	r4, r4, r3, lsr #16
	stmdb	r0!, {r4, r5, r12, lr}
	subs	r2, r2, #0x10         
	bge	.Lmemmove_bsrcul2loop16
	ldmia	sp!, {r4, r5, lr}
	adds	r2, r2, #0x0c         
	blt	.Lmemmove_bsrcul2l4

.Lmemmove_bsrcul2loop4:
	mov	r12, r3, lsl #16
	ldr	r3, [r1, #-4]!
	orr	r12, r12, r3, lsr #16
	str	r12, [r0, #-4]!
	subs	r2, r2, #4
	bge	.Lmemmove_bsrcul2loop4

.Lmemmove_bsrcul2l4:
	add	r1, r1, #2
	b	.Lmemmove_bl4

.Lmemmove_bsrcul1:
	cmp	r2, #0x0c            
	blt	.Lmemmove_bsrcul1loop4
	sub	r2, r2, #0x0c         
	stmdb	sp!, {r4, r5, lr}

.Lmemmove_bsrcul1loop32:
	mov	lr, r3, lsl #24
	ldmdb	r1!, {r3-r5, r12}
	orr	lr, lr, r12, lsr #8
	mov	r12, r12, lsl #24
	orr	r12, r12, r5, lsr #8
	mov	r5, r5, lsl #24
	orr	r5, r5, r4, lsr #8
	mov	r4, r4, lsl #24
	orr	r4, r4, r3, lsr #8
	stmdb	r0!, {r4, r5, r12, lr}
	subs	r2, r2, #0x10         
	bge	.Lmemmove_bsrcul1loop32
	ldmia	sp!, {r4, r5, lr}
	adds	r2, r2, #0x0c         
	blt	.Lmemmove_bsrcul1l4

.Lmemmove_bsrcul1loop4:
	mov	r12, r3, lsl #24
	ldr	r3, [r1, #-4]!
	orr	r12, r12, r3, lsr #8
	str	r12, [r0, #-4]!
	subs	r2, r2, #4
	bge	.Lmemmove_bsrcul1loop4

.Lmemmove_bsrcul1l4:
	add	r1, r1, #1
	b	.Lmemmove_bl4

FUNC memcpy
.Lnormal:
	stmdb	sp!, {r0, lr}		/* memcpy() returns dest addr */

	subs	r2, r2, #4
	blt	.Lmemcpy_l4		/* less than 4 bytes */
	ands	r12, r0, #3
	bne	.Lmemcpy_destul		/* oh unaligned destination addr */
	ands	r12, r1, #3
	bne	.Lmemcpy_srcul		/* oh unaligned source addr */

.Lmemcpy_t8:
	/* We have aligned source and destination */
	subs	r2, r2, #8
	blt	.Lmemcpy_l12		/* less than 12 bytes (4 from above) */
	subs	r2, r2, #0x14         
	blt	.Lmemcpy_l32		/* less than 32 bytes (12 from above) */
	stmdb	sp!, {r4}		/* borrow r4 */

	/* blat 32 bytes at a time */
	/* XXX for really big copies perhaps we should use more registers */
.Lmemcpy_loop32:	
	ldmia	r1!, {r3, r4, r12, lr}
	stmia	r0!, {r3, r4, r12, lr}
	ldmia	r1!, {r3, r4, r12, lr}
	stmia	r0!, {r3, r4, r12, lr}
	subs	r2, r2, #0x20         
	bge	.Lmemcpy_loop32

	cmn	r2, #0x10
	ldmgeia	r1!, {r3, r4, r12, lr}	/* blat a remaining 16 bytes */
	stmgeia	r0!, {r3, r4, r12, lr}
	subge	r2, r2, #0x10         
	ldmia	sp!, {r4}		/* return r4 */

.Lmemcpy_l32:
	adds	r2, r2, #0x14         

	/* blat 12 bytes at a time */
.Lmemcpy_loop12:
	ldmgeia	r1!, {r3, r12, lr}
	stmgeia	r0!, {r3, r12, lr}
	subges	r2, r2, #0x0c         
	bge	.Lmemcpy_loop12

.Lmemcpy_l12:
	adds	r2, r2, #8
	blt	.Lmemcpy_l4

	subs	r2, r2, #4
	ldrlt	r3, [r1], #4
	strlt	r3, [r0], #4
	ldmgeia	r1!, {r3, r12}
	stmgeia	r0!, {r3, r12}
	subge	r2, r2, #4

.Lmemcpy_l4:
	/* less than 4 bytes to go */
	adds	r2, r2, #4
	ldmeqia	sp!, {r0, pc}		/* done */
	/* copy the crud byte at a time */
	cmp	r2, #2
	ldrb	r3, [r1], #1
	strb	r3, [r0], #1
	ldrgeb	r3, [r1], #1
	strgeb	r3, [r0], #1
	ldrgtb	r3, [r1], #1
	strgtb	r3, [r0], #1
	ldmia	sp!, {r0, pc}

	/* erg - unaligned destination */
.Lmemcpy_destul:
	rsb	r12, r12, #4
	cmp	r12, #2

	/* align destination with byte copies */
	ldrb	r3, [r1], #1
	strb	r3, [r0], #1
	ldrgeb	r3, [r1], #1
	strgeb	r3, [r0], #1
	ldrgtb	r3, [r1], #1
	strgtb	r3, [r0], #1
	subs	r2, r2, r12
	blt	.Lmemcpy_l4		/* less the 4 bytes */

	ands	r12, r1, #3
	beq	.Lmemcpy_t8		/* we have an aligned source */

	/* erg - unaligned source */
	/* This is where it gets nasty ... */
.Lmemcpy_srcul:
	bic	r1, r1, #3
	ldr	lr, [r1], #4
	cmp	r12, #2
	bgt	.Lmemcpy_srcul3
	beq	.Lmemcpy_srcul2
	cmp	r2, #0x0c            
	blt	.Lmemcpy_srcul1loop4
	sub	r2, r2, #0x0c         
	stmdb	sp!, {r4, r5}

.Lmemcpy_srcul1loop16:
	mov	r3, lr, lsr #8
	ldmia	r1!, {r4, r5, r12, lr}
	orr	r3, r3, r4, lsl #24
	mov	r4, r4, lsr #8
	orr	r4, r4, r5, lsl #24
	mov	r5, r5, lsr #8
	orr	r5, r5, r12, lsl #24
	mov	r12, r12, lsr #8
	orr	r12, r12, lr, lsl #24
	stmia	r0!, {r3-r5, r12}
	subs	r2, r2, #0x10         
	bge	.Lmemcpy_srcul1loop16
	ldmia	sp!, {r4, r5}
	adds	r2, r2, #0x0c         
	blt	.Lmemcpy_srcul1l4

.Lmemcpy_srcul1loop4:
	mov	r12, lr, lsr #8
	ldr	lr, [r1], #4
	orr	r12, r12, lr, lsl #24
	str	r12, [r0], #4
	subs	r2, r2, #4
	bge	.Lmemcpy_srcul1loop4

.Lmemcpy_srcul1l4:
	sub	r1, r1, #3
	b	.Lmemcpy_l4

.Lmemcpy_srcul2:
	cmp	r2, #0x0c            
	blt	.Lmemcpy_srcul2loop4
	sub	r2, r2, #0x0c         
	stmdb	sp!, {r4, r5}

.Lmemcpy_srcul2loop16:
	mov	r3, lr, lsr #16
	ldmia	r1!, {r4, r5, r12, lr}
	orr	r3, r3, r4, lsl #16
	mov	r4, r4, lsr #16
	orr	r4, r4, r5, lsl #16
	mov	r5, r5, lsr #16
	orr	r5, r5, r12, lsl #16
	mov	r12, r12, lsr #16
	orr	r12, r12, lr, lsl #16
	stmia	r0!, {r3-r5, r12}
	subs	r2, r2, #0x10         
	bge	.Lmemcpy_srcul2loop16
	ldmia	sp!, {r4, r5}
	adds	r2, r2, #0x0c         
	blt	.Lmemcpy_srcul2l4

.Lmemcpy_srcul2loop4:
	mov	r12, lr, lsr #16
	ldr	lr, [r1], #4
	orr	r12, r12, lr, lsl #16
	str	r12, [r0], #4
	subs	r2, r2, #4
	bge	.Lmemcpy_srcul2loop4

.Lmemcpy_srcul2l4:
	sub	r1, r1, #2
	b	.Lmemcpy_l4

.Lmemcpy_srcul3:
	cmp	r2, #0x0c            
	blt	.Lmemcpy_srcul3loop4
	sub	r2, r2, #0x0c         
	stmdb	sp!, {r4, r5}

.Lmemcpy_srcul3loop16:
	mov	r3, lr, lsr #24
	ldmia	r1!, {r4, r5, r12, lr}
	orr	r3, r3, r4, lsl #8
	mov	r4, r4, lsr #24
	orr	r4, r4, r5, lsl #8
	mov	r5, r5, lsr #24
	orr	r5, r5, r12, lsl #8
	mov	r12, r12, lsr #24
	orr	r12, r12, lr, lsl #8
	stmia	r0!, {r3-r5, r12}
	subs	r2, r2, #0x10         
	bge	.Lmemcpy_srcul3loop16
	ldmia	sp!, {r4, r5}
	adds	r2, r2, #0x0c         
	blt	.Lmemcpy_srcul3l4

.Lmemcpy_srcul3loop4:
	mov	r12, lr, lsr #24
	ldr	lr, [r1], #4
	orr	r12, r12, lr, lsl #8
	str	r12, [r0], #4
	subs	r2, r2, #4
	bge	.Lmemcpy_srcul3loop4

.Lmemcpy_srcul3l4:
	sub	r1, r1, #1
	b	.Lmemcpy_l4
