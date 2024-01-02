#include "linked_list.h"
#include <stdlib.h>

node_t *create_node(void *data) {
  node_t *new_node = (node_t *)malloc(sizeof(node_t));
  if (new_node == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  new_node->data = data;
  new_node->prev = NULL;
  new_node->next = NULL;
  return new_node;
}

list_t *create_list() {
  list_t *list = (list_t *)malloc(sizeof(list_t));
  if (list == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    exit(EXIT_FAILURE);
  }
  list->head = NULL;
  list->tail = NULL;
  list->items_size = 0;
  return list;
}

void list_push(list_t *list, void *data) {
  node_t *new_node = create_node(data);
  if (list->head == NULL) {
    list->head = new_node;
    list->tail = new_node;
  } else {
    new_node->prev = list->tail;
    list->tail->next = new_node;
    list->tail = new_node;
  }
  list->items_size++;
}

void print_list_forward(list_t *list) {
  node_t *current = list->head;
  while (current != NULL) {
    printf("%p -> ", current->data);
    current = current->next;
  }
  printf("NULL\n");
}

void print_list_backward(list_t *list) {
  node_t *current = list->tail;
  while (current != NULL) {
    printf("%p -> ", current->data);
    current = current->prev;
  }
  printf("NULL\n");
}

void free_list(list_t *list) {
  node_t *current = list->head;
  node_t *next;
  while (current != NULL) {
    next = current->next;
    free(current);
    current = next;
  }
  free(list);
}
