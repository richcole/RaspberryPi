#include "list.h"
#include "malloc.h"

void list_new(struct list_t *lst) {
  lst->head = 0;
}

void list_add_first(struct list_t *lst, char *elem) {
  struct list_node_t *node = (struct list_node_t *)malloc_alloc(sizeof(struct list_node_t));
  if ( lst->head != 0 ) {
    node->prev = lst->head->prev;
    node->next = lst->head;
    lst->head->prev->next = node;
    lst->head->prev = node;
  }
  else {
    node->prev = node;
    node->next = node;
  }
  node->data = elem;
  lst->head = node;
}

void list_add_last(struct list_t *lst, char *elem) {
  struct list_node_t *node = (struct list_node_t *)malloc_alloc(sizeof(struct list_node_t));
  if ( lst->head != 0 ) {
    node->prev = lst->head->prev;
    node->next = lst->head;
    lst->head->prev->next = node;
    lst->head->prev = node;
  }
  else {
    node->prev = node;
    node->next = node;
  }
  node->data = elem;
}

struct list_node_t* list_first(struct list_t *lst) {
  return lst->head;
}

struct list_node_t* list_next(struct list_t *lst, struct list_node_t *node) {
  if ( node->next != lst->head ) {
    return node->next;
  }
  else {
    return 0;
  }
}

char *list_get(struct list_node_t *node) {
  return node->data;
}


