#include "list.h"
#include "malloc.h"
#include "uart.h"

void list_new(struct list_t *lst) {
  lst->head = 0;
}

void list_add_first(struct list_t *lst, void *elem) {
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

void list_add_last(struct list_t *lst, void *elem) {
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
    lst->head = node;
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

void *list_get(struct list_node_t *node) {
  return node->data;
}

void list_move_to_end(
  struct list_t* src, struct list_t* dst, struct list_node_t *node
) {
  node->prev->next = node->next;
  node->next->prev = node->prev;
  if ( src->head == node ) {
    if ( node->next != node ) {
      src->head = node->next;
    }
    else {
      src->head = 0;
    }
  }
  if ( dst->head == 0 ) {
    dst->head = node;
    node->next = node->prev = node;
  }
  else {
    node->prev = dst->head->prev;
    node->next = dst->head;
    dst->head->prev->next = node;
    dst->head->prev = node;
  }
}

void list_remove(struct list_t *lst, struct list_node_t *node) {
  if ( node->next == node ) {
    lst->head = 0;
  }
  else {
    node->prev->next = node->next;
    node->next->prev = node->prev;
    if ( lst->head == node ) {
      lst->head = node->next;
    }
  }
  malloc_free(node);
}

uint32 list_empty(struct list_t *lst) {
  return lst->head == 0;
};
