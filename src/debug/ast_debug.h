#ifndef _AST_DEBUG_
#define _AST_DEBUG_
#include "../ast.h"

typedef enum DUMP_TYPE_E DUMP_TYPE;
enum DUMP_TYPE_E {
  DUMP_TYPE_TERM = 0,
  DUMP_TYPE_GRAPHVIZ,
};

static int expr_size = 0;
static int lit_size = 0;
static int var_size = 0;
static int ret_size = 0;
static int scope_size = 0;
static int func_call_size = 0;
static int func_decl_size = 0;

void print_ast_ret(AST_return_t *ret, int level, DUMP_TYPE dump_type);
int sprint_ast_ret(char *str, AST_return_t *ret, int level, int margin,
                   DUMP_TYPE dump_type);

void print_ast_scope(FILE *file, AST_scope_t *scope, int level,
                     DUMP_TYPE dump_type);
int sprint_ast_scope(char *str, AST_scope_t *scope, int level, int margin,
                     DUMP_TYPE dump_type);

void print_ast_expr(AST_expr_t *expr, int level, DUMP_TYPE dump_type);
int sprint_ast_expr(char *str, AST_expr_t *expr, int level, int margin,
                    DUMP_TYPE dump_type);

void print_ast_ass(AST_assignment_t *ass, int level, DUMP_TYPE dump_type);
int sprint_ast_ass(char *str, AST_assignment_t *ass, int level, int margin,
                   DUMP_TYPE dump_type);

void print_ast_param(AST_param_t *param, int level, DUMP_TYPE dump_type);
int sprint_ast_param(char *str, AST_param_t *param, int level, int margin,
                     DUMP_TYPE dump_type);

void print_ast_arg(AST_arg_t *arg, int level, DUMP_TYPE dump_type);
int sprint_ast_arg(char *str, AST_arg_t *arg, int level, int margin,
                   DUMP_TYPE dump_type);

void print_ast_func_decl(AST_func_decl_t *func_decl, int level,
                         DUMP_TYPE dump_type);
int sprint_ast_func_decl(char *str, AST_func_decl_t *func_decl, int level,
                         int margin, DUMP_TYPE dump_type);

void print_ast_func_call(AST_func_call_t *func_call, int level,
                         DUMP_TYPE dump_type);
int sprint_ast_func_call(char *str, AST_func_call_t *func_call, int level,
                         int margin, DUMP_TYPE dump_type);
#endif