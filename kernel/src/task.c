#include "task.h"
#include "list.h"
#include "malloc.h"

struct list_t* active_tasks;
struct list_t* inactive_tasks;
struct list_node_t* current_task_it;

extern uint32 sp();

void task_init() {
  active_tasks = (struct list_t *)malloc_alloc(sizeof(struct list_t));
  list_new(active_tasks);

  inactive_tasks = (struct list_t *)malloc_alloc(sizeof(struct list_t));
  list_new(inactive_tasks);

  struct task_t *task = (struct task_t *)malloc_alloc(sizeof(struct task_t));
  task->sp    = sp();
  task->msg   = 0;
  task->pc    = 0;

  list_add_first(active_tasks, task);
  current_task_it = list_first(active_tasks); 
}

struct task_t *get_current_task() {
  return (struct task_t *)list_get(current_task_it);
}






