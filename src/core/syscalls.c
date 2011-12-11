#include "syscalls.h"
#include "alloc.h"

extern uint32_t __syscall_table;

extern void * switch_context_do;

// Define a syscall.
// Although this is a void function, it can return up to 4 values by cheating
// and setting reg[0]-reg[3]
// It's opaque, but it works.  See __alloc_cells() in lisp_memory.c for an
// example 
void define_syscall(syscall_linkage_t linkage, uint32_t number, void * func) {
	(&__syscall_table)[number & MAX_SYSCALLS] = (uint32_t)func | linkage;
}

void init_syscalls() {
  define_syscall(NAKED, SYS_YIELD, &switch_context_do);
}

void yield() {
  syscall0_nr(SYS_YIELD);
}
