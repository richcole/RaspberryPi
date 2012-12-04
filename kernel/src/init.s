/* FUNC macro */
.macro FUNC name
.text
.code 32
.global \name
\name:
.endm

start:
    ldr pc,reset_handler
    ldr pc,undefined_handler
    ldr pc,swi_handler
    ldr pc,prefetch_handler
    ldr pc,data_handler
    ldr pc,unused_handler
    ldr pc,irq_handler
    ldr pc,fiq_handler

reset_handler:      .word reset
undefined_handler:  .word hang
swi_handler:        .word hang
prefetch_handler:   .word hang
data_handler:       .word hang
unused_handler:     .word hang
irq_handler:        .word irq
fiq_handler:        .word hang

reset:
    /* copy reset vectors */
    ldr r0,=start
    mov r1,#0x0000
    ldmia r0!,{r2-r9}
    stmia r1!,{r2-r9}
    ldmia r0!,{r2-r9}
    stmia r1!,{r2-r9}

set_stacks:  
    /* firq */   
    mov r0,#0xD1
    msr cpsr,r0
    mov sp,#0x38000

    /* irq */
    mov r0,#0xD2
    msr cpsr,r0
    mov sp,#0x40000

    /* supervisor */
    mov r0,#0xD3
    msr cpsr,r0
    mov sp,#0x48000

    bl notmain

FUNC hang
    b hang

FUNC PUT32
    str r1,[r0]
    bx  lr

FUNC GET32
    ldr r0,[r0]
    bx  lr

FUNC dummy 
    bx  lr

FUNC flush_cache
    mov r0, #0x0000
    mcr p15, #0, r0, c7, c14, #0
    mov pc,lr

FUNC memory_barrier
    mov r0, #0x0000
    mcr p15, #0, r0, c7, c10, #5
    mov pc,lr

FUNC enable_irq
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr,r0
    bx  lr

FUNC disable_irq
    mrs r0,cpsr
    orr r0,r0,#0x80
    msr cpsr,r0
    bx  lr

FUNC sp
    mov r0,sp
    bx  lr

/* task_create(void **sp, void *lr) */
FUNC task_create
    push {r2,r3}
    ldr  r2, [r0]
    stmfd r2!, {r0-r1}
    stmfd r2!, {r2-r12}
    stmfd r2!, {r1}
    stmfd r2!, {r1}
    mrs   r3,cpsr
    stmfd r2!, {r3}
    str r2, [r0]
    pop {r2,r3}
    bx  lr

irq:
    push   {lr}
    bl     c_irq_handler
    pop    {lr}
    teq    r0,#0
    subeqs pc,r14,#4

task_switch_irq:
    sub    lr,lr,#4
    push   {r0,r1,r2}
    push   {lr}
    mov    r0,sp            /* save stack            */
    mrs    r1,spsr          /* swap to previous mode (FIXME user mode) */
    orr    r1,r1,#0x80      /*   with interupts off  */
    msr    cpsr,r1
    ldmfd  r0!,{r2}         /* load lr into r2 */
    push   {r2}             /* save pc */
    push   {r2}             /* save lr */
    push   {r3-r12}         /* save r3-r12 */
    ldmfd  r0!,{r2,r3,r4}   /* load r0,r1,r2 */
    push   {r2,r3,r4}       /* save r0,r1,r2 */
    push   {r1}             /* save cspr */
    mov    r0,sp
    b      task_yield_from_irq

/* task_switch(uint32 **sp, uint32 *new_sp) */
FUNC task_switch
    /* save current state */
    push {lr}             /* save pc            */
    push {r0-r12,lr}      /* save lr down to r0 */
    mrs  r2,cpsr          
    push {r2}             /* save cspr          */
    str  sp,[r0]          /* save sp in task    */
    mov  r0,r1            /* switch to new stack */
    /* continue on to task_switch_no_save */

/* task_switch_no_save(uint32 *sp) */
FUNC task_switch_no_save
    mov    sp,r0
    pop    {r0}
    bic    r0,r0,#0x80             /* enable interrupts */
    msr    spsr,r0
    ldmfd  sp!,{r0-r12,lr,pc}^     /* pop state and jump */

FUNC spsr
    mrs r0,spsr
    bx  lr

FUNC set_if_zero
    ldrex r2, [r0]
    cmp r2, #0
    strexeq r3, r1, [r0]
    mov r0, r3
    bx  lr

FUNC cpsr
    mrs r0,cpsr
    bx  lr

FUNC c1
    mrc p15,0,r0,c1,c0,1
    bx  lr

heap_end_ptr:                       .word
heap_start_ptr:                     .word   

