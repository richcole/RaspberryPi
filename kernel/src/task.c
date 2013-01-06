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
  char *p = malloc_alloc(0x800);
  p += 0x800;
  task->sp = (uint32 *)(p); // 2k stack
  task->msg = 0;
  task_create(&task->sp, task_func);
  list_add_last(active_tasks, task);
  print_buf("task start:");
  print_buf(" t=");
  print_ptr(task);
  print_buf(" t->sp=");
  print_ptr(task->sp);
  print_buf("\n");
  return task;
}

struct task_t *task_current() {
  return (struct task_t *)list_get(current_task_it);
}

void task_save_sp(uint32 *sp) {
  struct task_t *current_task = (struct task_t *)current_task_it->data;
  current_task->sp = sp;
}

void task_print(char *str, struct task_t *current_task, struct task_t *next_task) {
  print_buf(str);
  print_buf(" c=");
  print_ptr(current_task);
  print_buf(" c->sp=");
  print_ptr(current_task->sp);
  print_buf(" n=");
  print_ptr(next_task);
  print_buf(" n->sp=");
  print_ptr(next_task->sp);
  print_buf(" n->pc=");
  print_hex(next_task->sp[15]); // sp: cspr, 13 regs, lr, pc
  print_buf(" n->lr=");
  print_hex(next_task->sp[14]); // sp: cspr, 13 regs, lr, pc
  print_buf("\n");
};

void task_yield() {
  disable_irq();

  struct task_t *current_task = (struct task_t *)current_task_it->data;
  struct list_node_t *next_task_it = current_task_it->next;
  struct task_t *next_task = (struct task_t *)next_task_it->data;

  task_print("task_yield:", current_task, next_task);
  if ( current_task_it == next_task_it ) {
    return;
  };

  current_task_it = next_task_it;
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

  task_print("task_yield_and_move_to:", current_task, next_task);
  task_switch(&current_task->sp, next_task->sp);
}

void task_yield_from_irq(uint32 *sp) {
  struct list_node_t *next_task_it = current_task_it->next;
  struct task_t *next_task = (struct task_t *)next_task_it->data;
  struct task_t *current_task = (struct task_t *)current_task_it->data;

  current_task->sp = sp;
  current_task_it = next_task_it;

  task_print("task_yield_from_irq:", current_task, next_task);
  task_switch_no_save(next_task->sp);
}
