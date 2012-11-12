#include "malloc.h"
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

#include "mock.c"

int main() {
  void *free_space = malloc(50*1024*1024);
  malloc_init(free_space);
  char *ptr[2][1024];
  uint32 i, j, k, l;
  srand48(1L);

  for(i=0;i<1024;++i) {
    for(k=0;k<2;++k) {
      ptr[k][i] = 0;
    }
  }

  for(j=0;j<100000;++j) {
    for(k=0;k<2;k++) {
      for(i=0;i<1024;++i) {
        ptr[k][i] = malloc_alloc((rand() % 1024)+1);
      }
      fprintf(stdout, "%d\n", malloc_freelist_length());
      for(i=0;i<1024;++i) {
        l = (k + 1) % 2;
        if ( ptr[l][i] != 0 ) {
          malloc_free(ptr[l][i]);
          ptr[l][i] = 0;
        }
      }
      fprintf(stdout, "%d\n", malloc_freelist_length());
    }
  }
  return 0;
}
