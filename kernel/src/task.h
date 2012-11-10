#ifndef TASK_H
#define TASK_H

#include "types.h"
#include "buf.h"
#include "list.h"

struct task_t {
  uint32 sp;
  uint32 pc;
  struct buf_t  *msg;
};

void task_init();
struct task_t *get_current_task();

extern struct list_t* active_tasks;
extern struct list_t* inactive_tasks;
extern struct list_node_t* current_task_it;

#endif
