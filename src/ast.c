#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

AST_statement_t *create_ast_stmt(AST_TYPE type, void *stmt) {
  AST_statement_t *statement = malloc(sizeof(AST_statement_t));
  // TODO: there is some thing called union idiot
  statement->ast_type = type;
  switch (type) {
  case AST_ASS:
    statement->ass = stmt;
    break;
  case AST_EXPR_BIN:
  case AST_EXPR_NUM:
    statement->expr = stmt;
    break;
  case AST_SCOPE:
    statement->scope = stmt;
    break;
  case AST_RET:
    statement->ret = stmt;
    break;
  case AST_FUNC_DECL:
    statement->func_decl = stmt;
    break;
  case AST_FUNC_CALL:
    statement->func_call = stmt;
    break;
  case AST_FUNC_ARG_ID:
  case AST_FUNC_ARG_LIT:
    statement->arg = stmt;
    break;
  case AST_FUNC_PARAM:
    statement->param = stmt;
    break;
  case AST_IF_ELSE_STMT:
    statement->if_else_stmt = stmt;
    break;
  default:
    break;
  }
  return statement;
}

AST_scope_t *create_ast_scope() {
  AST_scope_t *scope = malloc(sizeof(AST_scope_t));
  scope->ast_type = AST_SCOPE;
  scope->statements = create_list(-1);
  return scope;
}
void scope_push(AST_scope_t *scope, AST_statement_t *stmt) {
  list_push(scope->statements, stmt);
}

AST_expr_t *create_ast_expr_var(token_t *id) {
  AST_expr_t *expr = malloc(sizeof(AST_expr_t));
  expr->ast_type = AST_EXPR_VAR;
  expr->id = id;
  return expr;
}
AST_expr_t *create_ast_expr_num(int value) {
  AST_expr_t *expr = malloc(sizeof(AST_expr_t));
  expr->ast_type = AST_EXPR_NUM;
  expr->value = value;
  return expr;
}
AST_expr_t *create_ast_expr_bin(OP op_type, AST_expr_t *lhs, AST_expr_t *rhs) {
  AST_expr_t *expr = malloc(sizeof(AST_expr_t));
  expr->ast_type = AST_EXPR_BIN;
  expr->op_type = op_type;
  expr->lhs = lhs;
  expr->rhs = rhs;
  return expr;
}
AST_expr_t *create_ast_expr_func_call(AST_func_call_t *func_call) {
  AST_expr_t *expr = malloc(sizeof(AST_expr_t));
  expr->ast_type = AST_EXPR_FUNC_CALL;
  expr->func_call = func_call;
  return expr;
}

AST_assignment_t *create_ast_ass(token_t *id, ID_TYPE id_type,
                                 AST_expr_t *value) {
  AST_assignment_t *ass = malloc(sizeof(AST_assignment_t));
  ass->ast_type = AST_ASS;
  ass->id = id;
  ass->id_type = id_type;
  ass->ass_expr = value;
  return ass;
}

AST_return_t *create_ast_ret(AST_expr_t *ret_expr) {
  AST_return_t *ret = malloc(sizeof(AST_return_t));
  ret->ast_type = AST_RET;
  ret->ret_expr = ret_expr;
  return ret;
}

AST_param_t *create_ast_param(token_t *id, ID_TYPE id_type) {
  AST_param_t *param = malloc(sizeof(AST_param_t));
  param->ast_type = AST_FUNC_PARAM;
  param->id = id;
  param->param_type = id_type;
  return param;
}

AST_arg_t *create_ast_arg_lit(int value) {
  AST_arg_t *arg = malloc(sizeof(AST_arg_t));
  arg->ast_type = AST_FUNC_ARG_LIT;
  arg->value = value;
  return arg;
}
AST_arg_t *create_ast_arg_var(token_t *id) {
  AST_arg_t *arg = malloc(sizeof(AST_arg_t));
  arg->ast_type = AST_FUNC_ARG_ID;
  arg->id = id;
  return arg;
}

AST_func_decl_t *create_ast_func_decl(token_t *id, list_t *params,
                                      AST_scope_t *body) {
  AST_func_decl_t *func_decl = malloc(sizeof(AST_func_decl_t));
  func_decl->ast_type = AST_FUNC_DECL;
  func_decl->id = id;
  func_decl->params = params;
  func_decl->body = body;
  return func_decl;
}

AST_func_call_t *create_ast_func_call(token_t *id) {
  AST_func_call_t *func_call = malloc(sizeof(AST_func_call_t));
  func_call->ast_type = AST_FUNC_CALL;
  func_call->args = create_list();
  func_call->id = id;
  return func_call;
}

AST_if_else_stmt_t *create_ast_if_else_stmt(AST_expr_t *expr,
                                            AST_scope_t *if_scope,
                                            AST_scope_t *else_scope) {
  AST_if_else_stmt_t *if_stmt = malloc(sizeof(AST_if_else_stmt_t));
  if_stmt->ast_type = AST_IF_ELSE_STMT;
  if_stmt->expr = expr;
  if_stmt->if_scope = if_scope;
  if_stmt->else_scope = else_scope;
  return if_stmt;
}

void free_ast_stmt(AST_statement_t *statement) {
  if (statement == NULL) {
    return;
  }

  free_ast_expr(statement->expr);
  free_ast_ass(statement->ass);
  free_ast_scope(statement->scope);
  free_ast_return(statement->ret);
  free_ast_func_decl(statement->func_decl);

  free(statement);
}

void free_ast_scope(AST_scope_t *scope) {
  if (scope == NULL) {
    return;
  }

  free_list(scope->statements);

  free(scope);
}

void free_ast_ass(AST_assignment_t *assignment) {
  if (assignment == NULL) {
    return;
  }

  free_ast_expr(assignment->ass_expr);
  free(assignment);
}

void free_ast_expr(AST_expr_t *expr) {
  if (expr == NULL) {
    return;
  }

  free(expr->text);
  free_ast_expr(expr->lhs);
  free_ast_expr(expr->rhs);

  free(expr);
}

void free_ast_return(AST_return_t *ret) {
  if (ret == NULL) {
    return;
  }

  free_ast_expr(ret->ret_expr);
  free(ret);
}

void free_ast_param(AST_param_t *param) {
  if (param == NULL) {
    return;
  }

  free(param->id);
  free(param);
}

void free_ast_arg(AST_arg_t *arg) {
  if (arg == NULL) {
    return;
  }

  free(arg->id);
  free(arg);
}
void free_ast_func_decl(AST_func_decl_t *func_decl) {
  if (func_decl == NULL) {
    return;
  }

  free(func_decl->id);
  node_t *current_param = func_decl->params->head;
  while (current_param != NULL) {
    AST_param_t *param = current_param->data;
    free_ast_param(param);
    current_param = current_param->next;
  }
  free_ast_scope(func_decl->body);

  free(func_decl);
}

void free_ast_func_call(AST_func_call_t *func_call) {
  if (func_call == NULL) {
    return;
  }

  free(func_call->id);
  node_t *current_arg = func_call->args->head;
  while (current_arg != NULL) {
    AST_arg_t *arg = current_arg->data;
    free_ast_arg(arg);
    current_arg = current_arg->next;
  }

  free_list(func_call->args);

  free(func_call);
}

void free_if_else_stmt(AST_if_else_stmt_t *if_stmt) {
  free_ast_expr(if_stmt->expr);
  free_ast_scope(if_stmt->if_scope);
  free_ast_scope(if_stmt->else_scope);
  free(if_stmt);
}