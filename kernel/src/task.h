#ifndef TASK_H
#define TASK_H

#include "types.h"
#include "msg.h"
#include "list.h"

typedef struct task_t {
  uint32* sp;
  struct msg_t msg;
} task_t;

typedef void (task_func_t)();

void task_init();
void task_yield();
void task_yield_and_move_to(struct list_t *dst_lst);
struct task_t *task_start(task_func_t *task_func);
void task_switch(uint32 **sp, uint32 *new_sp);
void task_print(struct task_t *task);

extern struct list_t* active_tasks;
extern struct list_t* inactive_tasks;
extern struct list_node_t* current_task_it;


#endif
