#include "task.h"
#include "list.h"
#include "malloc.h"
#include "uart.h"
#include "irq.h"
#include "channel.h"

#define TASK_STATE_ACTIVE 0x1

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
  task->state = TASK_STATE_ACTIVE;
  list_init(&task->input_channels);
  list_init(&task->output_channels);
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

uint32 task_is_active(struct task_t *task) {
  return task->state & TASK_STATE_ACTIVE;
}

void task_yield() {
  disable_irq();

  struct task_t *current_task = task_current();
  struct list_node_t *next_task_it;
  struct task_t *next_task;

  if ( task_is_active(current_task) ) {
    next_task_it = current_task_it->next;
  }
  else {
    next_task_it = list_first(active_tasks);
  }

  next_task = (struct task_t *)next_task_it->data;
  task_print("task_yield:", current_task, next_task);
  if ( current_task == next_task ) {
    return;
  };
  if ( next_task == 0 ) {
    print_buf("ERROR: next_task=0\n");
  };

  current_task_it = next_task_it;
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

void task_set_msg(struct msg_t *msg) {
  struct task_t *current_task = (struct task_t *)current_task_it->data;
  current_task->msg = msg;
}

struct task_t *task_make_inactive() {
  list_move_to_end(active_tasks, inactive_tasks, current_task_it);
  struct task_t *current_task = (struct task_t *)(current_task_it->data);
  current_task->state &= ~TASK_STATE_ACTIVE;
  return current_task;
}

void task_make_active(struct task_t *task) {
  struct list_node_t* task_it = list_find(inactive_tasks, task);
  list_move_to_end(inactive_tasks, active_tasks, task_it);
  task->state |= TASK_STATE_ACTIVE;
}

void task_add_input_channel(struct task_t *task, struct channel_t *ch) {
  list_add_last(&task->input_channels, ch);
}

void task_add_output_channel(struct task_t *task, struct channel_t *ch) {
  list_add_last(&task->output_channels, ch);
}

struct list_node_t* task_current_it() {
  return current_task_it;
}

struct task_t* task_current() {
  return (struct task_t *)current_task_it->data;
}



