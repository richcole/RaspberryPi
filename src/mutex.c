#include "mutex.h"

void mutex_acquire(mutex_t * mutex) {
  while (mutex_acquire_nolock(mutex) == 0xffffffff) {
    yield();
  }
}
