.include "macros.s"

save_context:
push {lr,r0-r12}
ldr  r0, [current_task]
str  sp, [r0]
bx          

next_task:
ldr  r0, [current_task, #4]
str  r0, [current_task]

load_context:
ldr  r0, [current_task]
ldr  sp, [r0]
pop  {lr,r0-r12}
bx   lr
        
        