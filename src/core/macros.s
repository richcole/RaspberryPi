.equ   MODE_BITS,     0x1F		 /*  Bit mask for mode bits in CPSR  */
.equ   USR_MODE,      0x10		 /*  User mode  */
.equ   FIQ_MODE,      0x11		 /*  Fast Interrupt Request mode  */
.equ   IRQ_MODE,      0x12		 /*  Interrupt Request mode  */
.equ   SVC_MODE,      0x13		 /*  Supervisor mode  */
.equ   ABT_MODE,      0x17		 /*  Abort mode */
        
.equ   UND_MODE,      0x1B		 /*  Undefined Instruction mode  */
.equ   SYS_MODE,      0x1F		 /*  System mode  */

.equ   IRQ_BASE,	  0x10140000 

.equ   IRQ_STATUS,    0x00
.equ   IRQ_ENABLE,    0x10
.equ   IRQ_ENCLEAR,   0x14
.equ   IRQ_SOFTCLEAR, 0x1c
.equ   IRQ_HANDLER,   0x30
.equ   IRQ_ACK,       0x30

/* FUNC macro */
.macro FUNC name
.text
.code 32
.global \name
\name:
.endm

