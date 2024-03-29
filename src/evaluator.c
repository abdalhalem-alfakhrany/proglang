#include "evaluator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "token.h"

evaluation_t *eval_scope(scope_context_t *context, AST_scope_t *scope) {
  node_t *current_stmt = scope->statements->head;
  evaluation_t *evaluation = malloc(sizeof(evaluation_t *));
  while (current_stmt != NULL) {
    AST_statement_t *stmt = (AST_statement_t *)current_stmt->data;
    switch (stmt->ast_type) {
    case AST_FUNC_DECL:
      evaluation = eval_func_decl(context, stmt->as.func_decl);
      break;
    case AST_FUNC_CALL:
      evaluation = eval_func_call(context, stmt->as.func_call);
      break;
    case AST_ASS:
      evaluation = eval_ass(context, stmt->as.ass);
      break;
    case AST_IF_ELSE_STMT:
      evaluation = eval_if_else(context, stmt->as.if_else_stmt);
      break;
    case AST_RET:
      evaluation = eval_ret(context, stmt->as.ret);
      break;
    default:
      printf("unknown statement (evaluation) %d\n", stmt->ast_type);
      break;
    }
    current_stmt = current_stmt->next;
  }
  return evaluation;
}

scope_context_t *create_context(scope_context_t *parent_context) {
  scope_context_t *context = malloc(sizeof(scope_context_t));
  context->functions = create_list();
  context->variables = create_list();
  context->children_contexts = create_list();
  context->parent_context = parent_context;
  if (parent_context != NULL)
    list_push(context->parent_context->children_contexts, context);
  return context;
}

void free_context(scope_context_t *context) {
  free_list(context->functions);
  free_list(context->variables);
  node_t *current_context = context->children_contexts->head;
  while (current_context != NULL) {
    free_context(current_context->data);
    current_context = current_context->next;
  }

  free(context->parent_context);
  free(context);
}

var_t *create_var(const char *name, int value) {
  var_t *var = malloc(sizeof(var_t));
  var->name = strdup(name);
  var->value = value;
  return var;
}

void free_var(var_t *var) {
  if (var != NULL) {
    free(var->name);
    free(var);
  }
}

var_t *get_var(scope_context_t *context, char *name) {
  node_t *current_var = context->variables->head;
  while (current_var != NULL) {
    var_t *var = (var_t *)current_var->data;
    if (!strcmp(name, var->name))
      return var;
    current_var = current_var->next;
  }
  if (context->parent_context == NULL)
    return NULL;
  return get_var(context->parent_context, name);
}

void push_var(scope_context_t *context, var_t *var) {
  list_push(context->variables, var);
}

func_t *create_func(const char *name, list_t *params, AST_scope_t *body,
                    scope_context_t *parent_context) {
  func_t *func = malloc(sizeof(func_t));
  func->name = strdup(name);
  func->params = params;
  func->context = create_context(parent_context);
  node_t *current_param = func->params->head;
  while (current_param != NULL) {
    AST_param_t *param = current_param->data;
    var_t *param_as_var = create_var(param->id->text, 0);
    if (param_as_var == NULL) {
      fprintf(stderr, "parameter %s not found in any higher scope !\n",
              param->id->text);
      exit(EXIT_FAILURE);
    }
    list_push(func->context->variables, param_as_var);
    current_param = current_param->next;
  }
  func->body = body;
  return func;
}

void free_func(func_t *func) {
  if (func != NULL) {
    free(func->name);
    free(func);
  }
}

void push_function(scope_context_t *context, func_t *func) {
  list_push(context->functions, func);
}

func_t *get_func(scope_context_t *context, char *name) {
  node_t *current_func = context->functions->head;
  while (current_func != NULL) {
    func_t *func = (func_t *)current_func->data;
    if (!strcmp(name, func->name))
      return func;
    current_func = current_func->next;
  }

  if (context->parent_context == NULL)
    return NULL;

  return get_func(context->parent_context, name);
}

evaluation_t *create_evaluation_ret(int value) {
  evaluation_t *evaluation = malloc(sizeof(evaluation_t));
  evaluation->eval_type = EVAL_TYPE_RET;
  evaluation->return_value = value;
  return evaluation;
}

evaluation_t *create_evaluation_ass() {
  evaluation_t *evaluation = malloc(sizeof(evaluation_t));
  evaluation->eval_type = EVAL_TYPE_ASS;
  return evaluation;
}

evaluation_t *create_evaluation_if_else() {
  evaluation_t *evaluation = malloc(sizeof(evaluation_t));
  evaluation->eval_type = EVAL_TYPE_IF_ELSE;
  return evaluation;
}

evaluation_t *create_evaluation_scope() {
  evaluation_t *evaluation = malloc(sizeof(evaluation_t));
  evaluation->eval_type = EVAL_SCOPE;
  return evaluation;
}

evaluation_t *create_evaluation_func_call() {
  evaluation_t *evaluation = malloc(sizeof(evaluation_t));
  evaluation->eval_type = EVAL_FUNC_CALL;
  return evaluation;
}

evaluation_t *create_evaluation_func_decl() {
  evaluation_t *evaluation = malloc(sizeof(evaluation_t));
  evaluation->eval_type = EVAL_FUNC_DECL;
  return evaluation;
}

int eval_expr(scope_context_t *context, AST_expr_t *expr) {
  if (expr->ast_type == AST_EXPR_NUM) {
    return expr->value;
  } else if (expr->ast_type == AST_EXPR_VAR) {
    return get_var(context, expr->id->text)->value;
  } else if (expr->ast_type == AST_EXPR_FUNC_CALL) {
    return eval_func_call(context, expr->func_call)->return_value;
  } else if (expr->ast_type == AST_EXPR_BIN) {
    int left_value = eval_expr(context, expr->lhs);
    int right_value = eval_expr(context, expr->rhs);

    switch (expr->op_type) {
    case OP_PLUS:
      return left_value + right_value;
    case OP_MINUS:
      return left_value - right_value;
    case OP_MULT:
      return left_value * right_value;
    case OP_DIV:
      return left_value / right_value;
    default:
      printf("Error: Invalid operator type\n");
      exit(1);
    }
  }
  return -1;
}

evaluation_t *eval_ass(scope_context_t *context, AST_assignment_t *ass) {
  list_push(context->variables,
            create_var(ass->id->text, eval_expr(context, ass->ass_expr)));
  return create_evaluation_ass();
}

evaluation_t *eval_func_decl(scope_context_t *context,
                             AST_func_decl_t *func_decl_ast) {
  list_push(context->functions,
            create_func(func_decl_ast->id->text, func_decl_ast->params,
                        func_decl_ast->body, context));
  return create_evaluation_func_decl();
}

evaluation_t *eval_ret(scope_context_t *context, AST_return_t *ret) {
  return create_evaluation_ret(eval_expr(context, ret->ret_expr));
}

evaluation_t *eval_if_else(scope_context_t *context,
                           AST_if_else_stmt_t *if_else_stmt) {
  int expr_eval = eval_expr(context, if_else_stmt->expr);

  evaluation_t *eval = create_evaluation_if_else();

  if (expr_eval == 1) {
    eval = eval_scope(context, if_else_stmt->if_scope);
  } else {
    if (if_else_stmt->elseif)
      eval = eval_if_else(context, if_else_stmt->elseif);

    if (if_else_stmt->else_scope)
      eval = eval_scope(context, if_else_stmt->else_scope);
  }
  return eval;
}

evaluation_t *eval_func_call(scope_context_t *context,
                             AST_func_call_t *func_call) {

  func_t *func = get_func(context, func_call->id->text);
  if (func == NULL) {
    fprintf(stderr, "no function named %s in context of scope\n",
            func_call->id->text);
    exit(EXIT_FAILURE);
  }

  node_t *current_arg = func_call->args->head;
  node_t *current_var = func->context->variables->head;
  while (current_arg != NULL) {

    AST_arg_t *arg = current_arg->data;
    var_t *var = current_var->data;

    var->value = arg->value;

    current_arg = current_arg->next;
    current_var = current_var->next;
  }

  evaluation_t *evaluation;
  node_t *current_stmt = func->body->statements->head;
  while (current_stmt != NULL) {
    AST_statement_t *stmt = current_stmt->data;
    switch (stmt->ast_type) {
    case AST_ASS:
      evaluation = eval_ass(func->context, stmt->as.ass);
      break;
    case AST_FUNC_CALL:
      evaluation = eval_func_call(func->context, stmt->as.func_call);
      break;
    case AST_RET:
      evaluation = eval_ret(func->context, stmt->as.ret);
      break;
    case AST_IF_ELSE_STMT:
      evaluation = eval_if_else(func->context, stmt->as.if_else_stmt);
      break;
    default:
      printf("unknown  statement (evaluation)\n");
      break;
    }
    if (evaluation->eval_type == EVAL_TYPE_RET) {
      return evaluation;
    }
    current_stmt = current_stmt->next;
  }

  return evaluation;
}