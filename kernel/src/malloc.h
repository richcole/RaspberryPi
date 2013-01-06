#ifndef MALLOC_H
#define MALLOC_H

#include "types.h"

void malloc_init(void *ptr);
void malloc_free(void *ptr);
char* malloc_alloc(uint32 size);
uint32 malloc_freelist_length();
void memcpy(void *src, void *dst, uint32 len);

#endif
