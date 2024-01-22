#ifndef _AST_
#define _AST_
#include "linked_list.h"
#include "token.h"

typedef enum AST_TYPE_E AST_TYPE;
enum AST_TYPE_E {
  AST_PROGRAM = 0,
  AST_ASS,
  AST_SCOPE,
  AST_FUNC_DECL,
  AST_FUNC_CALL,
  AST_FUNC_PARAM,
  AST_FUNC_ARG_ID,
  AST_FUNC_ARG_LIT,
  AST_EXPR_NUM,
  AST_EXPR_VAR,
  AST_EXPR_FUNC_CALL,
  AST_EXPR_BIN,
  AST_IF_STMT,
  AST_RET,
  AST_UNKOWN,
};

typedef enum ID_TYPE_E ID_TYPE;
enum ID_TYPE_E {
  ID_TYPE_INT = AST_UNKOWN + 1,
  ID_TYPE_FLOAT,
  ID_TYPE_UNKNOWN,
};

typedef struct AST_statement AST_statement_t;
typedef struct AST_scope AST_scope_t;
typedef struct AST_assignment AST_assignment_t;
typedef struct AST_expr AST_expr_t;
typedef struct AST_func_decl AST_func_decl_t;
typedef struct AST_func_call AST_func_call_t;
typedef struct AST_param AST_param_t;
typedef struct AST_arg AST_arg_t;
typedef struct AST_return AST_return_t;
typedef struct AST_if_stmt AST_if_stmt_t;

struct AST_statement {
  AST_TYPE ast_type;
  AST_expr_t *expr;
  AST_assignment_t *ass;
  AST_scope_t *scope;
  AST_return_t *ret;
  AST_func_decl_t *func_decl;
  AST_func_call_t *func_call;
  AST_param_t *param;
  AST_if_stmt_t *if_stmt;
  AST_arg_t *arg;
};

struct AST_scope {
  AST_TYPE ast_type;
  list_t *statements;
};

struct AST_assignment {
  AST_TYPE ast_type;
  token_t *id;
  ID_TYPE id_type;
  AST_expr_t *ass_expr;
};

struct AST_expr {
  AST_TYPE ast_type;
  token_t *id;
  int value;
  char *text;
  OP op_type;
  AST_func_call_t *func_call;
  AST_expr_t *lhs;
  AST_expr_t *rhs;
};

struct AST_return {
  AST_TYPE ast_type;
  AST_expr_t *ret_expr;
};

struct AST_param {
  AST_TYPE ast_type;
  ID_TYPE param_type;
  token_t *id;
};

struct AST_arg {
  AST_TYPE ast_type;
  token_t *id;
  int value;
};

struct AST_func_decl {
  AST_TYPE ast_type;
  token_t *id;
  list_t *params;
  AST_scope_t *body;
};

struct AST_func_call {
  AST_TYPE ast_type;
  token_t *id;
  list_t *args;
};

struct AST_if_stmt {
  AST_TYPE ast_type;
  AST_expr_t *expr;
  AST_scope_t *scope;
};

AST_statement_t *create_ast_stmt(AST_TYPE ast_type, void *stmt);

AST_scope_t *create_ast_scope();
void scope_push(AST_scope_t *scope, AST_statement_t *stmt);

AST_expr_t *create_ast_expr_bin(OP op_type, AST_expr_t *lhs, AST_expr_t *rhs);
AST_expr_t *create_ast_expr_var(token_t *id);
AST_expr_t *create_ast_expr_num(int value);
AST_expr_t *create_ast_expr_func_call(AST_func_call_t *func_call);

AST_assignment_t *create_ast_ass(token_t *id, ID_TYPE id_type,
                                 AST_expr_t *value);

AST_return_t *create_ast_ret(AST_expr_t *ret_expr);

AST_param_t *create_ast_param(token_t *id, ID_TYPE id_type);

AST_arg_t *create_ast_arg_var(token_t *id);
AST_arg_t *create_ast_arg_lit(int value);

AST_func_decl_t *create_ast_func_decl(token_t *id, list_t *params,
                                      AST_scope_t *body);

AST_func_call_t *create_ast_func_call(token_t *id);

AST_if_stmt_t *create_ast_if_stmt(AST_expr_t *expr, AST_scope_t *scope);

void free_ast_stmt(AST_statement_t *statement);
void free_ast_scope(AST_scope_t *scope);
void free_ast_ass(AST_assignment_t *assignment);
void free_ast_expr(AST_expr_t *expr);
void free_ast_return(AST_return_t *ret);
void free_ast_param(AST_param_t *param);
void free_ast_arg(AST_arg_t *param);
void free_ast_func_decl(AST_func_decl_t *func_decl);
void free_ast_func_call(AST_func_call_t *func_decl);
void free_if_stmt(AST_if_stmt_t *func_decl);

#endif