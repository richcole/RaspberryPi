#include "malloc.h"

extern char* __bss_end__;

struct malloc_entry_t {
  uint32 size;
  char   data[0];
};

struct malloc_heap_t {
  uint32 size;
  uint32 tail;
  struct malloc_entry_t  entries[0];
};

struct malloc_heap_t malloc_free_list = 0;
char *malloc_head = __bss_end__;

struct malloc_entry_t *entry malloc_get_entry(char *ptr) {
  return (struct malloc_entry_t *)(data - sizeof(malloc_entry_t));
}

void malloc_init() {
  uint32 size = 1024;
  malloc_free_list = (struct malloc_heap_t *) malloc_head;
  malloc_head += sizeof(malloc_heap_t) + sizeof(malloc_entry_t)*size;
  malloc_free_list->size = malloc_head;
  malloc_free_list->tail = 0;
}

void malloc_free(char *ptr) {
  struct malloc_entry_t *entry = malloc_get_entry(ptr);
  uint32 curr = malloc_free_list->tail++;
  uint32 parent = tail / 2;
  uint32 child;
  malloc_free_list->entries[curr] = entry;
  while( curr != 0 ) {
    if ( 
	malloc_free_list->entries[curr].size < 
	malloc_free_list->entries[parent].size 
    ) {
      malloc_free_list_swap(parent, curr);
      curr = parent;
    }
    else {
      break;
    }
    child = tail*2 + 1;
    if ( 
      malloc_free_list->entryes[child].size <
      malloc_free_list->entries[curr].size 
    ) {
      malloc_free_list_swap(child, curr);
    }
  }
}

void malloc_free_list_swap(uint32 x, uint32 y) {
  malloc_entry_t *tmp;
  tmp = malloc_free_list->entries[x];
  malloc_free_list->entries[x] = malloc_free_list->parent[y];
  malloc_free_list->entries[y] = tmp;
};

uint32 malloc_leq(uint32 left, uint32 right) {
  return malloc_free_list->entries[left].size =< 
    malloc_free_list->entries[right].size;
};

uint32 malloc_le(uint32 left, uint32 right) {
  return malloc_free_list->entries[left].size < 
    malloc_free_list->entries[right].size;
};

malloc_entry_t* malloc_free_list_remove(uint32 curr) {
  uint32 parent = curr / 2;
  uint32 left = parent*2 + 1;
  uint32 right = left + 1;
  while(parent != 0) {
    if ( malloc_leq
    left = curr*2+1;
    right = left + 1;
    if (
    {
      malloc_free_list_swap(left, curr);
    }
  }
}

char* malloc_alloc(uint32 size) {
  uint32 curr = 0;
  while( 
    curr < malloc_free_list->size && 
    malloc_free_list->entries[curr]->size < size 
  ) {
    curr = curr*2 + 1;
  }
  if ( curr < malloc_free_list->size ) {
    return malloc_free_list_remove(curr)->ptr;
  }
  else {
    return malloc_alloc_new_entry(size);
  }
}
