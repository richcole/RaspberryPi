#ifndef LIST_H
#define LIST_H

#include "types.h"

struct list_t;
struct list_node_t;

struct list_t {
  struct list_node_t *head;
};

struct list_node_t {
  struct list_node_t *prev;
  struct list_node_t *next;
  char *data;
};

void list_new(struct list_t *lst);
void list_add_first(struct list_t *lst, void *elem);
void list_add_last(struct list_t *lst, void *elem);
struct list_node_t* list_first(struct list_t *lst);
struct list_node_t* list_next(struct list_t *lst, struct list_node_t *node);
void *list_get(struct list_node_t *);
uint32 list_empty(struct list_t *lst);
void list_move_to_end(struct list_t* src, struct list_t* dst, struct list_node_t *node);

#define FOR_LIST_BEGIN(it, var, type, lst) \
  { \
    struct list_node_t* it; \
    for(it=list_first(lst); it != null; it=list_next(var)) {           \
      type var = (type) list_get(it); \

    
#define FOR_LIST_END \
  };

#endif
