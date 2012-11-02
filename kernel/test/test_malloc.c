#include "malloc.h"
#include <malloc.h>
#include <stdlib.h>

void test_malloc() {
  void *free_space = malloc(1024*1024);
  malloc_init(free_space);
  char *ptr[1024];
  uint32 i;
  srand48(1L);
  for(i=0;i<1024;++i) {
    ptr[i] = malloc_alloc(rand() % 1024);
  }
  for(i=0;i<1024;++i) {
    malloc_free(ptr[i]);
    ptr[i] = 0;
  }
  for(i=0;i<1024;++i) {
    ptr[i] = malloc_alloc(rand() % 1024);
  }
}
