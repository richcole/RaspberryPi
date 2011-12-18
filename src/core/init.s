.include "macros.s"
        
.section .reset, "ax"
.global __vector_table
.code 32

__vector_table:
ldr pc, reset_handler_address
ldr pc, undef_handler_address
ldr pc, siq_handler_address
ldr pc, prefetch_abort_handler_address
ldr pc, data_abort_handler_address
ldr pc, reserved_handler_address
ldr pc, irq_handler_address
ldr pc, fiq_handler_address

reset_handler_address:              .word   __reset_handler
undef_handler_address:              .word   __no_handler
siq_handler_address:                .word   __siq_handler
prefetch_abort_handler_address:     .word   __no_handler
data_abort_handler_address:         .word   __data_abort
reserved_handler_address:           .word   __no_handler
irq_handler_address:                .word   __irq_handler
fiq_handler_address:                .word   __fiq_handler

heap_end_ptr:                       .word
heap_start_ptr:                     .word   

/*  Make globals to allow remappings  */
.global reset_handler_address
.global undef_handler_address
.global siq_handler_address
.global prefetch_abort_handler_address
.global data_abort_handler_address
.global irq_handler_address
.global fiq_handler_address

.global heap_end_ptr
.global heap_start_ptr       

/*  Dummy handler that simply loops on itself  */
FUNC __no_handler
b __no_handler

FUNC __siq_handler
b __siq_handler

FUNC __data_abort
b __data_abort

FUNC __irq_handler
subs   lr,      lr,        #4          /* store the return position      */
push   {lr}                            /* save the return address        */

ldr    r3,      =__irq_handlers        /* base of irq handler array      */
ldr	   r4,      =IRQ_BASE              /* load the irq base              */
ldr    r0,      [r4, #IRQ_HANDLER]
ldr    r0,      [r4, #IRQ_STATUS]      /* r0 gets the status regs        */

ldr    r1,      =0x1                   /* search for first non zero irq  */
ldr    r2,      =0x0                   /* r1 gets shifted by r2          */

loop1:  
tst    r0,      r1, LSL r2             /* compare with status word       */
addeq  r2,      r2,        #1          /* increment if zero flag         */
beq    loop1                           /* jump back to loop1 unless zero */
add    r3,      r3,        r2
blxne  r3                              /* jump to r3 if not zero         */

str    r4,      [r4, #IRQ_ACK]         /* acknowledge interupt           */
pop    {lr}                            /* restore link register value    */
bx     lr                              /* return from interupt           */

FUNC __fiq_handler
subs   lr,      lr,        #4          /* fix the link register */
bx lr                                  /* return from interupt */


FUNC __reset_handler

mrs    r0,      cpsr                   /*  Original PSR value  */
ldr    r1,      =0x8000000             /*  128M is the top of memory */
ldr    r2,      =0x400                 /*  stack size */        

/*  IRQ Mode */
bic    r0,      r0,        #MODE_BITS  /*  Clear the mode bits  */
orr    r0,      r0,        #IRQ_MODE   /*  Set the mode */
msr    cpsr,    r0                     /*  Change the mode  */
mov    sp,      r1                     /*  set stack pointer */
sub    r1,      r1,        r2          /*  1k is the stack size         */

/*  SYS Mode */
bic    r0,      r0,        #MODE_BITS  /*  Clear the mode bits  */
orr    r0,      r0,        #SYS_MODE   /*  Set the mode  */
msr    cpsr,    r0                     /*  Change the mode */
mov    sp,      r1                     /*  set stack pointer */        
sub    r1,      r1,        r2          /*  1k is the stack size         */

/*  FIQ Mode */
bic    r0,      r0,        #MODE_BITS  /*  Clear the mode bits  */
orr    r0,      r0,        #FIQ_MODE   /*  Set the mode  */
msr    cpsr,    r0                     /*  Change the mode */
mov    sp,      r1                     /*  set stack pointer */
sub    r1,      r1,        r2          /*  1k is the stack size         */

/*  SVC Mode */
bic    r0,      r0,        #MODE_BITS  /*  Clear the mode bits  */
orr    r0,      r0,        #SVC_MODE   /*  Set the mode  */
msr    cpsr,    r0                     /*  Change the mode */
mov    sp,      r1                     /*  set stack pointer */
sub    r1,      r1,        r2          /*  1k is the stack size */

ldr    r3,      =heap_end_ptr           /*  store the heap end */
str    r1,      [r3]

ldr    r1,      =__bss_end__            /*  store the heap start */
ldr    r3,      =heap_start_ptr
str    r1,      [r3]        
        
cpsie  i                               /*  enable irq  */
cpsie  f                               /*  and fiq  */

ldr    r1,     =c_entry                /*  Jump to c entry */
mov    lr,     pc
bx     r1


