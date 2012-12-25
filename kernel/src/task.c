#include "task.h"
#include "list.h"
#include "malloc.h"
#include "uart.h"
#include "irq.h"

struct list_t* active_tasks;
struct list_t* inactive_tasks;
struct list_node_t* current_task_it;

void task_switch_no_save(uint32 *new_sp);
void task_create(uint32 **sp, task_func_t *lr);

void task_init() {
  active_tasks = (struct list_t *)malloc_alloc(sizeof(struct list_t));
  list_init(active_tasks);

  inactive_tasks = (struct list_t *)malloc_alloc(sizeof(struct list_t));
  list_init(inactive_tasks);

  struct task_t *task = (struct task_t *)malloc_alloc(sizeof(struct task_t));
  task->sp    = 0;
  task->msg   = 0;

  list_add_first(active_tasks, task);
  current_task_it = list_first(active_tasks);
}

struct task_t *task_start(task_func_t *task_func) {
  struct task_t *task = (struct task_t *)malloc_alloc(sizeof(struct task_t));
  print_buf("task start task=");
  print_ptr(task);

  char *p = malloc_alloc(2048);
  p += 2040;
  task->sp = (uint32 *)(p); // 2k stack
  /* task->msg = 0; */
  print_buf(" &sp=");
  print_ptr(&task->sp);
  print_buf(" sp=");
  print_ptr(task->sp);
  print_buf(" func=");
  print_ptr(task_func);
  print_buf(" msg=");
  print_ptr(task->msg);
  print_buf("\n");
  task_create(&task->sp, task_func);
  list_add_last(active_tasks, task);
  return task;
}

struct task_t *task_current() {
  return (struct task_t *)list_get(current_task_it);
}

void task_save_sp(uint32 *sp) {
  struct task_t *current_task = (struct task_t *)current_task_it->data;
  current_task->sp = sp;
}

void task_yield() {
  disable_irq();

  struct task_t *current_task = (struct task_t *)current_task_it->data;
  struct list_node_t *next_task_it = current_task_it->next;
  struct task_t *next_task = (struct task_t *)next_task_it->data;

  if ( current_task_it == next_task_it ) {
    return;
  };

  current_task_it = next_task_it;
  print_buf("task_yield next_task->sp[15]=");
  print_hex(next_task->sp[15]);
  print_buf("\n");

  task_switch(&current_task->sp, next_task->sp);
}

void task_yield_and_move_to(struct list_t *dst_lst) {
  disable_irq();
  struct task_t *current_task = (struct task_t *)current_task_it->data;
  struct list_node_t *next_task_it = current_task_it->next;
  struct task_t *next_task = (struct task_t *)next_task_it->data;

  if ( current_task_it == next_task_it ) {
    print_buf("Error cannot remove the last task\n");
    return;
  };

  list_move_to_end(active_tasks, dst_lst, current_task_it);
  current_task_it = next_task_it;

  print_buf("task_yield_and_move_to next_task->sp[15]=");
  print_hex(next_task->sp[15]);
  print_buf("\n");

  task_switch(&current_task->sp, next_task->sp);
}

void task_print(struct task_t *task) {
  print_buf("task sp=");
  print_ptr(task->sp);
  print_buf(" msg=");
  print_ptr(task->msg);
  print_buf(" task pc=");
  print_hex(task->sp[15]);
  print_buf(" task lr=");
  print_hex(task->sp[14]);
  print_buf("\n");
}

void task_yield_from_irq(uint32 *sp) {
  struct list_node_t *next_task_it = current_task_it->next;
  struct task_t *next_task = (struct task_t *)next_task_it->data;
  struct task_t *current_task = (struct task_t *)current_task_it->data;

  current_task->sp = sp;
  current_task_it = next_task_it;

  print_buf("task_yield_from_irq sp=");
  print_ptr(sp);
  print_buf(" sp[15]=");
  print_hex(sp[15]);
  print_buf(" sp[19]=");
  print_hex(sp[19]);
  print_buf(" next_task->sp[15]=");
  print_hex(next_task->sp[15]);
  print_buf("\n");

  task_switch_no_save(next_task->sp);
}
