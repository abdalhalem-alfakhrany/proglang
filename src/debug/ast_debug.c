#include "ast_debug.h"
#include <stdio.h>
#include <stdlib.h>

#include "../utils.h"

void print_ast_scope(FILE *file, AST_scope_t *scope, int level,
                     DUMP_TYPE dump_type) {
  char buff[2048];
  sprint_ast_scope(buff, scope, level, 0, dump_type);
  fprintf(file, "%s\n", buff);
}
int sprint_ast_scope(char *str, AST_scope_t *scope, int level, int margin,
                     DUMP_TYPE dump_type) {
  int n = 0;
  if (dump_type == DUMP_TYPE_TERM) {
    PRINT_TABS(0)
    scope_size++;
    node_t *current_stmt = scope->statements->head;
    while (current_stmt != NULL) {

      AST_statement_t *stmt = (AST_statement_t *)current_stmt->data;

      switch (stmt->ast_type) {
      case AST_ASS:
        n += sprint_ast_ass(str + n, stmt->ass, level + 1, margin, dump_type);
        break;
      case AST_EXPR_NUM:
      case AST_EXPR_VAR:
      case AST_EXPR_FUNC_CALL:
      case AST_EXPR_BIN:
        n += sprint_ast_expr(str + n, stmt->expr, level + 1, margin, dump_type);
        break;
      case AST_RET:
        n += sprint_ast_ret(str + n, stmt->ret, level + 1, margin, dump_type);
        break;
      case AST_FUNC_DECL:
        n += sprint_ast_func_decl(str + n, stmt->func_decl, level + 1, margin,
                                  dump_type);
        break;
      case AST_FUNC_CALL:
        n += sprint_ast_func_call(str + n, stmt->func_call, level + 1, margin,
                                  dump_type);
        break;
      case AST_SCOPE:
        n += sprint_ast_scope(str + n, stmt->scope, level + 1, margin,
                              dump_type);
        break;
      default:
        printf("unknown type %d\n", stmt->ast_type);
        break;
      }
      n += sprintf(str + n, "\n");
      if (current_stmt->next != NULL) {
        PRINT_TABS(1)
        n += sprintf(str + n, ",\n");
      }
      current_stmt = current_stmt->next;
    }
    PRINT_TABS(0)
    n += sprintf(str + n, "]\n");
  } else if (dump_type == DUMP_TYPE_GRAPHVIZ) {
    n += sprintf(
        str + n,
        "digraph {\ngraph [bgcolor = \"#1E1E1E\";];\nnode [color = white; "
        "fontcolor = white;];\nedge [color = white; fontcolor = white;];\n");
    node_t *current_stmt = scope->statements->head;
    while (current_stmt != NULL) {
      AST_statement_t *stmt = (AST_statement_t *)current_stmt->data;
      switch (stmt->ast_type) {
      case AST_FUNC_DECL:
        n += sprint_ast_func_decl(str + n, stmt->func_decl, level + 1, margin,
                                  dump_type);
        break;
      case AST_FUNC_CALL:
        n += sprint_ast_func_call(str + n, stmt->func_call, level + 1, margin,
                                  dump_type);
        break;
      case AST_SCOPE:
        n += sprint_ast_scope(str + n, stmt->scope, level + 1, margin,
                              dump_type);
        break;
      default:
        printf("unknown type %d\n", stmt->ast_type);
        break;
      }
      n += sprintf(str + n, "\n");
      current_stmt = current_stmt->next;
    }
    n += sprintf(str + n, "}\n");
  }
  return n;
}

void print_ast_expr(AST_expr_t *expr, int level, DUMP_TYPE dump_type) {
  char buff[256];
  sprint_ast_expr(buff, expr, level, 0, dump_type);
  printf("%s", buff);
}
int sprint_ast_expr(char *str, AST_expr_t *expr, int level, int margin,
                    DUMP_TYPE dump_type) {
  int n = 0;
  if (dump_type == DUMP_TYPE_TERM) {
    PRINT_TABS(0)
    if (expr->ast_type == AST_EXPR_NUM)
      n += sprintf(str + n, "int literal `%d`", expr->value);
    else if (expr->ast_type == AST_EXPR_VAR)
      n += sprintf(str + n, "variable `%s`", expr->id->text);
    else if (expr->ast_type == AST_EXPR_FUNC_CALL)
      n += sprintf(str + n, "function call `%s`", expr->func_call->id->text);
    else if (expr->ast_type == AST_EXPR_BIN) {
      n += sprintf(str + n, "< binary expresion %c\n", op_char(expr->op_type));
      n += sprint_ast_expr(str + n, expr->lhs, level + 1, margin, dump_type);
      n += sprintf(str + n, "\n");
      n += sprint_ast_expr(str + n, expr->rhs, level + 1, margin, dump_type);
      n += sprintf(str + n, "\n");
      PRINT_TABS(0)
      n += sprintf(str + n, ">");
    }
  } else if (dump_type == DUMP_TYPE_GRAPHVIZ) {
    if (expr->ast_type == AST_EXPR_NUM) {
      lit_size++;
      n += sprintf(str + n, "lit%d[label=\"%d\";];", lit_size, expr->value);
    } else if (expr->ast_type == AST_EXPR_VAR) {
      var_size++;
      n += sprintf(str + n, "var%d[label=\"%s\";];", var_size, expr->id->text);
    } else if (expr->ast_type == AST_EXPR_FUNC_CALL) {
      func_call_size++;
      n += sprint_ast_func_call(str + n, expr->func_call, level + 1, margin,
                                dump_type);
      // n += sprintf(str + n, "func_call%d[label=\"%s\";];", func_call_size,
      //              expr->func_call->id->text);
    } else if (expr->ast_type == AST_EXPR_BIN) {
      expr_size++;
      n += sprintf(str + n, "expr%d[label=\"%c\";];", expr_size,
                   op_char(expr->op_type));

      n += sprint_ast_expr(str + n, expr->lhs, level + 1, margin, dump_type);
      if (expr->lhs->ast_type == AST_EXPR_NUM) {
        n += sprintf(str + n, "expr%d->lit%d;", expr_size, lit_size);
      } else if (expr->lhs->ast_type == AST_EXPR_VAR) {
        n += sprintf(str + n, "expr%d->var%d;", expr_size, var_size);
      }

      n += sprint_ast_expr(str + n, expr->rhs, level + 1, margin, dump_type);
      if (expr->rhs->ast_type == AST_EXPR_NUM) {
        n += sprintf(str + n, "expr%d->lit%d;", expr_size, lit_size);
      } else if (expr->rhs->ast_type == AST_EXPR_VAR) {
        n += sprintf(str + n, "expr%d->var%d;", expr_size, var_size);
      }
    }
  }
  return n;
}

void print_ast_ass(AST_assignment_t *ass, int level, DUMP_TYPE dump_type) {
  char buff[256];
  sprint_ast_ass(buff, ass, level, 0, dump_type);
  printf("%s\n", buff);
}
int sprint_ast_ass(char *str, AST_assignment_t *ass, int level, int margin,
                   DUMP_TYPE dump_type) {
  int n = 0;
  char buff[256];
  if (dump_type == DUMP_TYPE_TERM) {
    PRINT_TABS(0)
    if (ass->ass_expr->ast_type == AST_EXPR_NUM ||
        ass->ass_expr->ast_type == AST_EXPR_VAR) {
      sprint_ast_expr(buff, ass->ass_expr, 0, margin, dump_type);
      n += sprintf(str + n, "< assignment '%s' `%s` %s>", ass->id->text,
                   type_str(ass->id_type), buff);
    } else if (ass->ass_expr->ast_type == AST_EXPR_BIN) {
      sprint_ast_expr(buff, ass->ass_expr, level + 1, margin, dump_type);
      n += sprintf(str + n, "< assignment '%s' `%s` \n%s>", ass->id->text,
                   type_str(ass->id_type), buff);
    }
  } else if (dump_type == DUMP_TYPE_GRAPHVIZ) {
    var_size++;
    sprint_ast_expr(buff, ass->ass_expr, 0, margin, dump_type);
    if (ass->ass_expr->ast_type == AST_EXPR_BIN) {
      n += sprintf(str + n, "%s\nvar%d[label=\"%s %s\"];var%d->expr%d;", buff,
                   var_size, type_str(ass->id_type), ass->id->text, var_size,
                   expr_size);
    } else if (ass->ass_expr->ast_type == AST_EXPR_NUM) {
      n += sprintf(str + n, "%s\nvar%d[label=\"%s %s\"];var%d->lit%d;", buff,
                   var_size, type_str(ass->id_type), ass->id->text, var_size,
                   lit_size);
    }
  }
  return n;
}

void print_ast_ret(AST_return_t *ret, int level, DUMP_TYPE dump_type) {
  char buff[256];
  sprint_ast_ret(buff, ret, level, 0, dump_type);
  printf("%s\n", buff);
}
int sprint_ast_ret(char *str, AST_return_t *ret, int level, int margin,
                   DUMP_TYPE dump_type) {
  int n = 0;
  char buff[256];

  if (dump_type == DUMP_TYPE_TERM) {
    PRINT_TABS(0)
    if (ret->ret_expr->ast_type == AST_EXPR_BIN) {
      sprint_ast_expr(buff, ret->ret_expr, level + 1, margin, dump_type);
      n += sprintf(str + n, "< return \n%s\n", buff);
      PRINT_TABS(0)
      n += sprintf(str + n, ">");
    } else if (ret->ret_expr->ast_type == AST_EXPR_NUM ||
               ret->ret_expr->ast_type == AST_EXPR_FUNC_CALL) {
      sprint_ast_expr(buff, ret->ret_expr, 0, margin, dump_type);
      n += sprintf(str + n, "< return %s>", buff);
    }
  } else if (dump_type == DUMP_TYPE_GRAPHVIZ) {
    ret_size++;
    sprint_ast_expr(buff, ret->ret_expr, level + 1, margin, dump_type);
    if (ret->ret_expr->ast_type == AST_EXPR_NUM) {
      n += sprintf(str + n, "%s\nret%d[label=\"return\";];ret%d->lit%d;\n",
                   buff, ret_size, ret_size, lit_size);
    } else if (ret->ret_expr->ast_type == AST_EXPR_VAR) {
      n += sprintf(str + n, "%s\nret%d[label=\"return\";];ret%d->var%d;\n",
                   buff, ret_size, ret_size, var_size);
    } else if (ret->ret_expr->ast_type == AST_EXPR_FUNC_CALL) {
      n +=
          sprintf(str + n, "%s\nret%d[label=\"return\";];ret%d->func_call%d;\n",
                  buff, ret_size, ret_size, func_call_size);
    } else if (ret->ret_expr->ast_type == AST_EXPR_BIN) {
      n += sprintf(str + n, "%s\nret%d[label=\"return\";];ret%d->expr%d;\n",
                   buff, ret_size, ret_size, expr_size);
    }
  }
  return n;
}

void print_ast_param(AST_param_t *param, int level, DUMP_TYPE dump_type) {
  char buff[256];
  sprint_ast_param(buff, param, level, 0, dump_type);
  printf("%s\n", buff);
}
int sprint_ast_param(char *str, AST_param_t *param, int level, int margin,
                     DUMP_TYPE dump_type) {
  int n = 0;
  if (dump_type == DUMP_TYPE_TERM) {
    PRINT_TABS(0)
    n += sprintf(str + n, "< param `%s` `%s` >", param->id->text,
                 type_str(param->param_type));
  } else if (dump_type == DUMP_TYPE_GRAPHVIZ) {
    n +=
        sprintf(str + n, "%s %s", type_str(param->param_type), param->id->text);
  }
  return n;
}

void print_ast_arg(AST_arg_t *arg, int level, DUMP_TYPE dump_type) {
  char buff[256];
  sprint_ast_arg(buff, arg, level, 0, dump_type);
  printf("%s\n", buff);
}
int sprint_ast_arg(char *str, AST_arg_t *arg, int level, int margin,
                   DUMP_TYPE dump_type) {
  int n = 0;
  if (dump_type == DUMP_TYPE_TERM) {
    PRINT_TABS(0)
    n += sprintf(str + n, "< function argument `%s`>", arg->id->text);
  } else if (dump_type == DUMP_TYPE_GRAPHVIZ) {
    n += sprintf(str + n, "%s", arg->id->text);
  }
  return n;
}

void print_ast_func_decl(AST_func_decl_t *func_decl, int level,
                         DUMP_TYPE dump_type) {
  char buff[256];
  sprint_ast_func_decl(buff, func_decl, level, 0, dump_type);
  printf("%s\n", buff);
}
int sprint_ast_func_decl(char *str, AST_func_decl_t *func_decl, int level,
                         int margin, DUMP_TYPE dump_type) {
  int n = 0;

  if (dump_type == DUMP_TYPE_TERM) {
    PRINT_TABS(0)
    n += sprintf(str + n, "< function declaration `%s` ", func_decl->id->text);

    if (func_decl->params->items_size <= 3) {
      node_t *current_param = func_decl->params->head;
      while (current_param != NULL) {
        AST_param_t *param = (AST_param_t *)current_param->data;
        n += sprint_ast_param(str + n, param, 0, margin, dump_type);
        current_param = current_param->next;
        if (current_param != NULL)
          n += sprintf(str + n, ", ");
      }
    } else {
      n += sprintf(str + n, "\n");
      PRINT_TABS(1)
      n += sprintf(str + n, "(\n");
      node_t *current_param = func_decl->params->head;
      while (current_param != NULL) {
        AST_param_t *param = (AST_param_t *)current_param->data;
        n += sprint_ast_param(str + n, param, level + 2, margin, dump_type);
        current_param = current_param->next;
        PRINT_TABS(1)

        n += sprintf(str + n, "\n");
      }
      PRINT_TABS(1)
      n += sprintf(str + n, ")\n");
    }
    n += sprintf(str + n, "\n");
    n += sprint_ast_scope(str + n, func_decl->body, level, margin, dump_type);
    PRINT_TABS(0)
    n += sprintf(str + n, ">");
  } else if (dump_type == DUMP_TYPE_GRAPHVIZ) {
    func_decl_size++;

    n += sprintf(str + n, "func_decl%d[label=\"%s: (", func_decl_size,
                 func_decl->id->text);

    node_t *current_param = func_decl->params->head;
    while (current_param != NULL) {
      AST_param_t *param = (AST_param_t *)current_param->data;
      n += sprint_ast_param(str + n, param, 0, margin, dump_type);
      current_param = current_param->next;
      if (current_param != NULL)
        n += sprintf(str + n, ", ");
    }
    n += sprintf(str + n, ")\";shape=box;];\n");
    node_t *current_stmt = func_decl->body->statements->head;
    while (current_stmt != NULL) {
      AST_statement_t *stmt = (AST_statement_t *)current_stmt->data;
      switch (stmt->ast_type) {
      case AST_RET:
        n += sprint_ast_ret(str + n, stmt->ret, level, margin, dump_type);
        n += sprintf(str + n, "func_decl%d->ret%d;", func_decl_size, ret_size);
        break;
      case AST_ASS:
        n += sprint_ast_ass(str + n, stmt->ass, level, margin, dump_type);
        n += sprintf(str + n, "func_decl%d->var%d;", func_decl_size, var_size);
        break;
      default:
        break;
      }
      current_stmt = current_stmt->next;
    }
  }

  return n;
}

void print_ast_func_call(AST_func_call_t *func_call, int level,
                         DUMP_TYPE dump_type) {
  char buff[256];
  sprint_ast_func_call(buff, func_call, level, 0, dump_type);
  printf("%s\n", buff);
}
int sprint_ast_func_call(char *str, AST_func_call_t *func_call, int level,
                         int margin, DUMP_TYPE dump_type) {
  int n = 0;
  if (dump_type == DUMP_TYPE_TERM) {
    PRINT_TABS(0)
    n += sprintf(str + n, "< function call `%s` ", func_call->id->text);

    if (func_call->args->items_size <= 3) {
      node_t *current_arg = func_call->args->head;
      while (current_arg != NULL) {
        AST_arg_t *arg = (AST_arg_t *)current_arg->data;
        n += sprint_ast_arg(str + n, arg, 0, margin, dump_type);
        if (current_arg->next != NULL)
          n += sprintf(str + n, " ,");
        current_arg = current_arg->next;
      }
      n += sprintf(str + n, ">");
    } else {
      n += sprintf(str + n, "\n");
      PRINT_TABS(1);
      n += sprintf(str + n, "(\n");
      node_t *current_arg = func_call->args->head;
      while (current_arg != NULL) {
        AST_arg_t *arg = (AST_arg_t *)current_arg->data;
        n += sprint_ast_arg(str + n, arg, level + 2, margin, dump_type);
        PRINT_TABS(1)
        n += sprintf(str + n, "\n");
        current_arg = current_arg->next;
      }

      PRINT_TABS(1)
      n += sprintf(str + n, ")\n");
      PRINT_TABS(0)
      n += sprintf(str + n, ">");
    }
  } else if (dump_type == DUMP_TYPE_GRAPHVIZ) {
    func_call_size++;
    n += sprintf(str + n, "func_call%d[label=\"%s: (", func_call_size,
                 func_call->id->text);

    node_t *current_arg = func_call->args->head;
    while (current_arg != NULL) {
      AST_arg_t *arg = (AST_arg_t *)current_arg->data;
      switch (arg->ast_type) {
      case AST_FUNC_ARG_LIT:
        n += sprintf(str + n, "%d", arg->value);
        break;
      case AST_FUNC_ARG_ID:
        n += sprintf(str + n, "%s", arg->id->text);
        break;
      default:
        break;
      }

      if (current_arg->next != NULL)
        n += sprintf(str + n, " ,");

      current_arg = current_arg->next;
    }

    n += sprintf(str + n, ")\";shape=cds];");
  }
  return n;
}
