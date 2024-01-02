#ifndef _STACK_
#define _STACK_
#include <stddef.h>
#include <stdbool.h>

typedef struct stack stack_t;
struct stack {
  int *items;
  size_t stack_size;
  size_t capacity;
};

stack_t *create_stack(size_t capacity);
void stack_push(stack_t *stack, int value);
int stack_pop(stack_t *stack);
int stack_top(stack_t *stack);
bool stack_empty(stack_t *stack);
#endif