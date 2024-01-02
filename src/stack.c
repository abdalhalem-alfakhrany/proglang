#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

stack_t *create_stack(size_t capacity) {
  stack_t *stack = malloc(sizeof(stack_t));
  stack->capacity = ((int)capacity == -1) ? 256 : capacity;
  stack->items = malloc(sizeof(int) * stack->capacity);
  stack->stack_size = 0;
  return stack;
}

void stack_push(stack_t *stack, int value) {
  if (stack->stack_size >= stack->capacity)
    stack->items = realloc(stack->items, (256));
  stack->items[stack->stack_size++] = value;
}

int stack_pop(stack_t *stack) {
  if (stack->stack_size <= 0) {
    printf("stack is empty\n");
    return 0;
  } else {
    stack->items[stack->stack_size] = 0;
    return stack->items[--stack->stack_size];
  }
}

int stack_top(stack_t *stack) { return stack->items[stack->stack_size - 1]; }
bool stack_empty(stack_t *stack) { return stack->stack_size == 0; }