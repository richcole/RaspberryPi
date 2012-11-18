#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "malloc.h"
#include "list.h"
#include "mock.c"

int verify_hello_world(struct list_t *lst) {
  struct list_node_t *lst_it = list_first(lst);
  if ( strcmp("Hello", list_get(lst_it)) != 0 ) {
    return 1;
  }
  lst_it = list_next(lst, lst_it);
  if ( strcmp("World", list_get(lst_it)) != 0 ) {
    return 1;
  }
  return 0;
}

int main() {
  void *free_space = malloc(50*1024*1024);
  malloc_init(free_space);

  struct list_t *lst = (struct list_t *)malloc_alloc(sizeof(struct list_t));
  list_new(lst);
  list_add_last(lst, "Hello");
  list_add_last(lst, "World");
  
  FOR_LIST_BEGIN(lst_it, str, char *, lst);
  fprintf(stdout, "%s\n", str);
  FOR_LIST_END();

  if ( verify_hello_world(lst) != 0 ) {
    return 1;
  }

  list_remove(lst, list_first(lst));
  list_remove(lst, list_first(lst));

  if ( list_first(lst) != 0 ) {
    return 1;
  }

  list_add_first(lst, "Hello");
  list_add_last(lst, "World");

  if ( verify_hello_world(lst) != 0 ) {
    return 1;
  }

  return 0;
}
