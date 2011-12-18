#include "task.h"

void yield() {
  save_context();
  current_task = current_task->next;
  load_context();
}
