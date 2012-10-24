#include "task.h"

struct task_t {
  uint32 sp;
};

struct task_list_t {
  struct task_list_t *next;
  struct task_list_t *prev;
  struct task_t *task;
};

struct task_list_t *active_tasks = 0;
struct task_list_t *blocked_tasks = 0;
struct task_list_t *curr_task = 0;

extern void switch_to(uint32 sp);
extern uint switch_from();

void switch_to_next_task(struct task_t *task) {
  if ( curr_task == 0 || curr_task->next == 0 ) {
    curr_task = active_tasks;
  }
  else {
    curr_task = curr_task->next;
  }
  if ( curr_task != 0 ) {
    switch_to(curr_task->sp);
  }
}

