#include "tasks.h"

task_t *current_task;

void save_context();
void load_context();

void yield() {
  save_context();
  current_task = current_task->next;
  load_context();
}
