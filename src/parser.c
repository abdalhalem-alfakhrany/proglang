#include "parser.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

parser_t *create_parser(lexer_t *lexer) {
  parser_t *parser = malloc(sizeof(parser_t));
  parser->lexer = lexer;
  parser->current_token = NULL;
  return parser;
}

void free_parser(parser_t *parser) { free(parser); }

void parser_advance(parser_t *parser) {
  if (parser->current_token)
    free_token(parser->current_token);
  token_t *token = lexer_next_token(parser->lexer);
  parser->current_token = token;
}

int parser_expect(parser_t *parser, ...) {
  va_list args;
  va_start(args, parser);

  int token_type;
  while ((token_type = va_arg(args, int)) != -1) {
    if (parser->current_token->token_type == token_type) {
      va_end(args);
      return 1;
    }
  }
  va_end(args);
  return 0;
}

AST_expr_t *parse_primary(parser_t *parser) {
  AST_expr_t *primary = NULL;

  if (parser_expect(parser, TOKEN_NUMBER, TOKEN_ID, TOKEN_LPAREN, -1)) {
    if (parser->current_token->token_type == TOKEN_NUMBER) {
      primary = create_ast_expr_num(atoi(parser->current_token->text));
      parser_advance(parser);
      return primary;
    }

    if (parser->current_token->token_type == TOKEN_ID) {
      token_t *id = parser->current_token;
      AST_func_call_t *function_call = parse_func_call_ast(parser);

      if (function_call != NULL)
        primary = create_ast_expr_func_call(function_call);
      else
        primary = create_ast_expr_var(id);

      // parser_advance(parser);
      return primary;
    }

    parser_advance(parser);
    primary = parse_expr_ast(parser, 0);

    EXPECT_ERROR(TOKEN_RPAREN);
    parser_advance(parser);
    return primary;
  } else {
    fprintf(stderr, "%s:%d:%d: error: expect id or integer value here\n", LOC);
  }
  return primary;
}

AST_expr_t *parse_expr_ast(parser_t *parser, int min_prec) {
  AST_expr_t *lhs = parse_primary(parser);

  while (is_operator(parser->current_token->token_type) &&
         prec(parser->current_token->token_type) >= min_prec) {
    int op_type = parser->current_token->token_type;
    parser_advance(parser);

    AST_expr_t *rh = parse_expr_ast(parser, prec(op_type) + 1);

    AST_expr_t *binary_expr = create_ast_expr_bin(op_type, lhs, rh);
    lhs = binary_expr;
  }

  return lhs;
}

AST_assignment_t *parse_assignment_ast(parser_t *parser) {
  AST_assignment_t *ass;

  parser_advance(parser);
  token_t *id = parser->current_token;

  parser_advance(parser);
  EXPECT_ERROR(TOKEN_EQUAL)
  parser_advance(parser);

  AST_expr_t *value = parse_expr_ast(parser, 0);

  ass = create_ast_ass(id, ID_TYPE_INT, value);
  EXPECT_ERROR(TOKEN_SIMI)
  parser_advance(parser);

  return ass;
}

AST_return_t *parse_return_ast(parser_t *parser) {
  AST_return_t *ret;
  parser_advance(parser);
  AST_expr_t *ret_expr = parse_expr_ast(parser, 0);
  ret = create_ast_ret(ret_expr);
  EXPECT_ERROR(TOKEN_SIMI)
  parser_advance(parser);

  return ret;
}

AST_scope_t *parse_program_ast(parser_t *parser) {
  AST_scope_t *scope = create_ast_scope();

  parser_advance(parser);
  while (!parser_expect(parser, TOKEN_EOF, -1)) {
    if (parser->current_token->token_type == TOKEN_RET) {
      AST_return_t *ret = parse_return_ast(parser);
      scope_push(scope, create_ast_stmt(ret->ast_type, ret));
    } else if (is_type(parser->current_token->text) != ID_TYPE_UNKNOWN) {
      AST_assignment_t *ass = parse_assignment_ast(parser);
      scope_push(scope, create_ast_stmt(ass->ast_type, ass));
    } else {
      free_token(parser->current_token);
      scope_push(scope,
                 create_ast_stmt(AST_FUNC_DECL, parse_func_decl_ast(parser)));
      parser_advance(parser);
    }
  }
  free_token(parser->current_token);

  return scope;
}

AST_scope_t *parse_func_body_ast(parser_t *parser) {
  AST_scope_t *scope = create_ast_scope();
  parser_advance(parser);
  while (!parser_expect(parser, TOKEN_RPRACE, TOKEN_EOF, -1)) {
    if (parser->current_token->token_type == TOKEN_IF) {
      AST_if_else_stmt_t *if_stmt = parse_if_else_stmt_ast(parser);
      scope_push(scope, create_ast_stmt(if_stmt->ast_type, if_stmt));
    } else if (is_type(parser->current_token->text) != ID_TYPE_UNKNOWN) {
      AST_assignment_t *ass = parse_assignment_ast(parser);
      scope_push(scope, create_ast_stmt(ass->ast_type, ass));
    } else if (parser->current_token->token_type == TOKEN_RET) {
      AST_return_t *ret = parse_return_ast(parser);
      scope_push(scope, create_ast_stmt(ret->ast_type, ret));
    } else {
      scope_push(scope,
                 create_ast_stmt(AST_FUNC_CALL, parse_func_call_ast(parser)));
      parser_advance(parser);
    }
  }

  return scope;
}

AST_param_t *parse_param_ast(parser_t *parser) {
  EXPECT_ERROR(TOKEN_ID);
  AST_param_t *param;
  token_t *param_id;
  ID_TYPE type;

  if (!strcmp("int", parser->current_token->text))
    type = ID_TYPE_INT;

  parser_advance(parser);
  param_id = parser->current_token;

  param = create_ast_param(param_id, type);
  parser_advance(parser);

  return param;
}

AST_arg_t *parse_arg_ast(parser_t *parser) {
  if (!parser_expect(parser, TOKEN_NUMBER, TOKEN_ID, -1)) {
    fprintf(stderr, "%s:%d:%d: error: expect id ot literal here\n", LOC);
    exit(EXIT_FAILURE);
  }

  AST_arg_t *arg;

  if (parser->current_token->token_type == TOKEN_NUMBER)
    arg = create_ast_arg_lit(atoi(parser->current_token->text));
  else if (parser->current_token->token_type == TOKEN_ID)
    arg = create_ast_arg_var(parser->current_token);

  parser_advance(parser);
  return arg;
}

AST_func_decl_t *parse_func_decl_ast(parser_t *parser) {
  AST_scope_t *body;
  list_t *params = create_list();
  token_t *func_id = malloc(sizeof(token_t));
  memcpy(parser->current_token, func_id, sizeof(token_t));

  parser_advance(parser);
  EXPECT_ERROR(TOKEN_LPAREN);
  while (1) {
    parser_advance(parser);

    if (parser->current_token->token_type == TOKEN_ID) {
      list_push(params, parse_param_ast(parser));
    }

    if (!parser_expect(parser, TOKEN_COMMA, TOKEN_RPAREN, -1)) {
      fprintf(stderr, "%s:%d:%d: error: expect `,` or `)` \n", LOC);
      exit(EXIT_FAILURE);
    }

    if (parser->current_token->token_type == TOKEN_RPAREN)
      break;
  }

  parser_advance(parser);
  body = parse_func_body_ast(parser);
  AST_func_decl_t *func = create_ast_func_decl(func_id, params, body);

  return func;
}

AST_func_call_t *parse_func_call_ast(parser_t *parser) {
  EXPECT_ERROR(TOKEN_ID);
  token_t *id = parser->current_token;
  list_t *args = create_list();
  parser_advance(parser);
  if (parser->current_token->token_type != TOKEN_LPAREN) {
    free_list(args);
    return NULL;
  }
  while (1) {
    parser_advance(parser);
    list_push(args, parse_arg_ast(parser));
    if (!parser_expect(parser, TOKEN_COMMA, TOKEN_RPAREN, -1)) {
      fprintf(stderr, "%s:%d:%d: error: expect `,` or `)` \n", LOC);
      exit(EXIT_FAILURE);
    }
    if (parser->current_token->token_type == TOKEN_RPAREN)
      break;
  }
  parser_advance(parser);

  AST_func_call_t *func_call = create_ast_func_call(id, args);
  return func_call;
}

AST_scope_t *parse_ctrl_scope_ast(parser_t *parser) {
  AST_scope_t *scope = create_ast_scope();

  while (!parser_expect(parser, TOKEN_RPRACE, TOKEN_EOF, -1)) {
    if (is_type(parser->current_token->text) != ID_TYPE_UNKNOWN) {
      AST_assignment_t *ass = parse_assignment_ast(parser);
      scope_push(scope, create_ast_stmt(ass->ast_type, ass));
    } else if (parser->current_token->token_type == TOKEN_RET) {
      AST_return_t *ret = parse_return_ast(parser);
      scope_push(scope, create_ast_stmt(ret->ast_type, ret));
    } else {
      scope_push(scope, create_ast_stmt(AST_IF_ELSE_STMT,
                                        parse_func_call_ast(parser)));
      parser_advance(parser);
    }
  }

  return scope;
}

AST_if_else_stmt_t *parse_if_else_stmt_ast(parser_t *parser) {
  EXPECT_ERROR(TOKEN_IF)
  parser_advance(parser);
  EXPECT_ERROR(TOKEN_LPAREN)
  parser_advance(parser);
  AST_expr_t *expr = parse_expr_ast(parser, 0);
  EXPECT_ERROR(TOKEN_RPAREN)
  parser_advance(parser);

  EXPECT_ERROR(TOKEN_LPRACE)
  parser_advance(parser);
  AST_scope_t *if_scope = parse_ctrl_scope_ast(parser);
  AST_scope_t *else_scope = NULL;
  AST_if_else_stmt_t *elseif = NULL;
  EXPECT_ERROR(TOKEN_RPRACE)

  parser_advance(parser);
  if (parser->current_token->token_type == TOKEN_ELSE) {
    parser_advance(parser);
    if (parser->current_token->token_type == TOKEN_IF) {
      elseif = parse_if_else_stmt_ast(parser);
    } else if (parser->current_token->token_type == TOKEN_LPRACE) {
      parser_advance(parser);
      else_scope = parse_ctrl_scope_ast(parser);
      parser_advance(parser);
    }
  }

  AST_if_else_stmt_t *if_stmt =
      create_ast_if_else_stmt(expr, if_scope, else_scope, elseif);
  return if_stmt;
}