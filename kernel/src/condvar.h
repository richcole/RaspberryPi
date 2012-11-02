#ifndef LOCKS_H
#define LOCKS_H

#include "types.h"
#include "task.h"

struct condvar_t;

void condvar_new(struct condvar_t *condvar);
void condvar_lock(struct condvar_t *condvar, struct task_t *task);
void condvar_unlock(struct condvar_t *condvar, struct task_t *task);
void condvar_wait(struct condvar_t *condvar);
void condvar_notify(struct condvar_t *condvar);
void condvar_notify_all(struct condvar_t *condvar);

#endif
