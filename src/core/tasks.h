#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>

struct Task {
  size_t sp;
  task_t *next;
  task_t *prev;
};

typedef struct Task task_t;

extern task_t *current_task;

#endif
