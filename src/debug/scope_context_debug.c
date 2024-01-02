#include "scope_context_debug.h"
#include "../utils.h"

int sprint_context(char str, scope_context_t *context, int level, int margin) {
  int n = 0;
  node_t *current_context = context->children_contexts->head;
  while (current_context != NULL) {
    sprint_context(str + n, context->children_contexts->head->next, level + 1);
    current_context = current_context->next;
  }
  scope_context_t *context = current_context->data;

  PRINT_TABS(0);
  n += printf(str + n, "functions ->\n");
  node_t *current_func = context->functions->head;
  while (current_func != NULL) {
    func_t *func = current_func->data;
    PRINT_TABS(0);
    n += printf(str + n, "%s\n", func->name);
    current_func = current_func->next;
  }

  PRINT_TABS(0);
  n += printf(str + n, "variables ->\n");
  node_t *current_var = context->variables->head;
  while (current_var != NULL) {
    var_t *var = current_var->data;
    PRINT_TABS(0);
    n += printf(str + n, "%s\n", var->name);
    current_var = current_var->next;
  }
  n += printf(str + n, "\n");
}
