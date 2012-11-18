#include "task.h"
#include "list.h"
#include "malloc.h"
#include "uart.h"

struct list_t* active_tasks;
struct list_t* inactive_tasks;
struct list_node_t* current_task_it;

extern void* sp();
extern void task_switch(void **sp, void *new_sp);
extern void task_create(void **sp, task_func_t *lr);

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

void task_print_active()
{
  print_buf("printing tasks: sp=");
  print_ptr(sp());
  print_buf("\n");

  FOR_LIST_BEGIN(task_it, task, struct task_t *, active_tasks);
  if ( task_it == current_task_it ) {
    print_buf("* ");
  }
  print_buf("task=");
  print_ptr(task);
  print_buf(", sp=");
  print_ptr(task->sp);
  print_buf("\n");
  FOR_LIST_END();
};

struct task_t *task_start(task_func_t *task_func) {
  struct task_t *task = (struct task_t *)malloc_alloc(sizeof(struct task_t));
  task->sp = (malloc_alloc(2048) + 2048); // 2k stack
  task_create(&task->sp, task_func);
  list_add_last(active_tasks, task);
  return task;
}

struct task_t *task_current() {
  return (struct task_t *)list_get(current_task_it);
}

void task_yield() {
  disable_irq();
  struct task_t *current_task = (struct task_t *)current_task_it->data;
  struct list_node_t *next_task_it = current_task_it->next;
  struct task_t *next_task = (struct task_t *)next_task_it->data;
  current_task_it = next_task_it;
  task_switch(&current_task->sp, next_task->sp);
}

void task_yield_from_irq() {
  struct task_t *current_task = (struct task_t *)current_task_it->data;
  struct list_node_t *next_task_it = current_task_it->next;
  struct task_t *next_task = (struct task_t *)next_task_it->data;
  current_task_it = next_task_it;
  task_switch(&current_task->sp, next_task->sp);
}
