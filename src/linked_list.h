#ifndef _DOUBLY_LINKED_LIST_
#define _DOUBLY_LINKED_LIST_
#include <stdio.h>
#include <stdlib.h>

typedef struct node node_t;
struct node {
  void *data;
  node_t *prev;
  node_t *next;
};

typedef struct list list_t;
struct list {
  node_t *head;
  node_t *tail;
  size_t items_size;
};

node_t *create_node(void *data);
list_t *create_list();
void list_push(list_t *list, void *data);
void print_list_forward(list_t *list);
void print_list_backward(list_t *list);
void free_list(list_t *list);

#endif
