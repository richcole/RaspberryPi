#include "condvar.h"
#include "task.h"
#include "list.h"

struct condvar_t {
  struct list_t waiting;
  struct task_t *holder;
};

void condvar_new(struct condvar_t *condvar) {
  list_new(&condvar->waiting);
  condvar->holder = 0;
};

extern void set_if_zero(void *ptr, void *val);

void condvar_lock(struct condvar_t *condvar, struct task_t *task) {
  set_if_zero(condvar->holder, task);
  while ( condvar->holder != task ) {
    list_add_last(&condvar->waiting, task);
    task_set_blocked(task);
  }
}

void condvar_unlock(struct condvar_t *condvar, struct task_t *task) {
}
