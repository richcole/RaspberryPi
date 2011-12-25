.include "macros.s"

.global current_task

FUNC save_context
push {r0-r12,lr}
ldr  r0, =current_task
ldr  r0, [r0]
str  sp, [r0]
bx   lr

FUNC next_task
ldr  r0, =current_task
ldr  r1, [r0, #4]        
str  r1, [r0]

FUNC load_context
ldr  r0, =current_task
ldr  r0, [r0]        
ldr  sp, [r0]
pop  {r0-r12,lr}
bx   lr
        
        