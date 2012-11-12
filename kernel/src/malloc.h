#ifndef MALLOC_H
#define MALLOC_H

#include "types.h"

void malloc_init(void *ptr);
void malloc_free(char *ptr);
char* malloc_alloc(uint32 size);
uint32 malloc_freelist_length();

#endif
