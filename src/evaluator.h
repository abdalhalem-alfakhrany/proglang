#ifndef _EVALUATOR_
#define _EVALUATOR_
#include "ast.h"

typedef enum EVAL_TYPE_E EVAL_TYPE;
enum EVAL_TYPE_E {
  EVAL_TYPE_RET = 0,
  EVAL_TYPE_ASS,
  EVAL_SCOPE,
  EVAL_FUNC_CALL,
  EVAL_FUNC_DECL,
  EVAL_TYPE_IF_ELSE,
};

typedef struct var var_t;
typedef struct scope_context scope_context_t;
typedef struct evaluation evaluation_t;
typedef struct func func_t;

struct var {
  char *name;
  int value;
};

struct func {
  char *name;
  list_t *params;
  scope_context_t *context;
  AST_scope_t *body;
};

struct evaluation {
  EVAL_TYPE eval_type;
  int return_value;
};

struct scope_context {
  list_t *variables;
  list_t *functions;
  list_t *children_contexts;
  scope_context_t *parent_context;
};

evaluation_t *eval_scope(scope_context_t *context, AST_scope_t *scope);

var_t *get_var(scope_context_t *context, char *name);
func_t *get_func(scope_context_t *context, char *name);

scope_context_t *create_context(scope_context_t *parent_context);
void free_context();

var_t *create_var(const char *name, int value);
void free_var(var_t *var);
void push_var(scope_context_t *context, var_t *var);

func_t *create_func(const char *name, list_t *params, AST_scope_t *body,
                    scope_context_t *parent_context);
void free_func(func_t *func);
void push_func(scope_context_t *context, func_t *func);

evaluation_t *create_evaluation_ret(int value);
evaluation_t *create_evaluation_ass();
evaluation_t *create_evaluation_if_else();
evaluation_t *create_evaluation_scope();

evaluation_t *eval_ass(scope_context_t *context, AST_assignment_t *ass);
evaluation_t *eval_func_decl(scope_context_t *context,
                             AST_func_decl_t *func_decl_ast);
evaluation_t *eval_func_call(scope_context_t *context,
                             AST_func_call_t *func_call);
int eval_term(scope_context_t *context, AST_term_t *term);
int eval_expr(scope_context_t *context, AST_expr_t *expr);
evaluation_t *eval_ret(scope_context_t *context, AST_return_t *ret);
evaluation_t *eval_if_else(scope_context_t *context,
                           AST_if_else_stmt_t *if_else_stmt);

#endif