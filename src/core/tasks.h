#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>

typedef struct Task task_t;

struct Task {
  size_t sp;
  task_t *next;
  task_t *prev;
};

extern task_t *current_task;
void yield();

#endif
