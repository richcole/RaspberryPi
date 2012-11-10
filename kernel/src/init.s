/* FUNC macro */
.macro FUNC name
.text
.code 32
.global \name
\name:
.endm

FUNC _start
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
    mov r0,#0x8000
    mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}


    ;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD2
    msr cpsr_c,r0
    mov sp,#0x8000

    ;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD1
    msr cpsr_c,r0
    mov sp,#0x4000

    ;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD3
    msr cpsr_c,r0
    mov sp,#0x8000000

    ;@ SVC MODE, IRQ ENABLED, FIQ DIS
    ;@mov r0,#0x53
    ;@msr cpsr_c, r0

    bl notmain

FUNC hang
    b hang

FUNC PUT32
    str r1,[r0]
    bx lr

FUNC GET32
    ldr r0,[r0]
    bx lr

FUNC dummy 
    bx lr

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
    msr cpsr_c,r0
    bx lr

FUNC sp
    mov r0,sp

irq:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_irq_handler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4

FUNC switch_to
    mov  sp,r0
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4

FUNC switch_from
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    mov  r0,sp

FUNC spsr
    mrs r0,spsr
    bx lr

FUNC set_if_zero
    ldrex r2, [r0]
    cmp r2, #0
    strexeq r3, r1, [r0]
    mov r0, r3
    bx lr

FUNC cpsr
    mrs r0,cpsr
    bx lr

FUNC c1
    mrc p15,0,r0,c1,c0,1
    bx lr

heap_end_ptr:                       .word
heap_start_ptr:                     .word   

