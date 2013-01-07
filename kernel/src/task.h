#ifndef TASK_H
#define TASK_H

#include "types.h"
#include "msg.h"
#include "list.h"

struct channel_t;

struct task_t {
  uint32* sp;
  struct msg_t *msg;
  struct list_t input_channels;
  struct list_t output_channels;
  uint32 state;
};

typedef void (task_func_t)();

void task_init();
void task_yield();
struct task_t *task_start(task_func_t *task_func);
void task_switch(uint32 **sp, uint32 *new_sp);
void task_print(char *hdr, struct task_t *task, struct task_t *next);
void task_set_msg(struct msg_t *msg);
struct task_t* task_current();
struct list_node_t* task_current_it();

struct task_t *task_make_inactive();
void task_make_active(struct task_t *task);

void task_add_input_channel(struct task_t *task, struct channel_t *ch);
void task_add_output_channel(struct task_t *task, struct channel_t *ch);



#endif
