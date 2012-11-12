#include "task.h"
#include "list.h"
#include "malloc.h"

struct list_t* active_tasks;
struct list_t* inactive_tasks;
struct list_node_t* current_task_it;

extern void switch_to();
extern void switch_to_no_save();
extern void task_create(uint32 sp, task_func_t *lr);

void task_init() {
  active_tasks = (struct list_t *)malloc_alloc(sizeof(struct list_t));
  list_new(active_tasks);

  inactive_tasks = (struct list_t *)malloc_alloc(sizeof(struct list_t));
  list_new(inactive_tasks);

  struct task_t *task = (struct task_t *)malloc_alloc(sizeof(struct task_t));
  task->sp    = 0;
  task->msg   = 0;

  list_add_first(active_tasks, task);
  current_task_it = list_first(active_tasks);
}

struct task_t *task_start(task_func_t task_func) {
  struct task_t *task = (struct task_t *)malloc_alloc(sizeof(struct task_t));
  task->sp = malloc_alloc(2048); // 2k stack
  task_create(sp, &task_func);
}

void task_func(task_func_t *fun) {
  fun();

  struct list_node_t* next_task_it = current_task_it->next;
  struct task_t* current_task = current_task_it->data;
  struct task_t* next_task = next_task_it->data;

  list_remove(active_tasks, current_task_it);
  malloc_free(current_task);
  switch_to_no_save(next_task->sp);
}

struct task_t *task_current() {
  return (struct task_t *)list_get(current_task_it);
}

void task_yield() {
  struct task_t *current_task = (struct task_t *)current_task_it->data;
  struct list_node_t *next_task_it = current_task_it->next;
  struct task_t *current_task = (struct task_t *)next_task_it->data;
  current_task_it = next_task_it;
  switch_to(&current_task->sp, next_task->sp);
}





