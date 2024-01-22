#ifndef _PARSER_
#define _PARSER_
#include "ast.h"
#include "lexer.h"
#include "token.h"
#define LOC                                                                    \
  "examples/control_statements.prog", parser->current_token->col, parser->current_token->row

typedef struct parser parser_t;
struct parser {
  lexer_t *lexer;
  list_t *tokens;
  token_t *current_token;
  token_t *previous_token;
};

parser_t *create_parser(lexer_t *lexer);
int parser_expect(parser_t *parser, ...);
void parser_advance(parser_t *parser);

AST_expr_t *parse_expr_ast(parser_t *parser, int precedence);
AST_expr_t *parse_primary(parser_t *parser);
AST_assignment_t *parse_assignment_ast(parser_t *parser);

AST_return_t *parse_return_ast(parser_t *parser);

AST_scope_t *parse_program_ast(parser_t *parser);
AST_scope_t *parse_func_body_ast(parser_t *parser);

AST_param_t *parse_param_ast(parser_t *parser);
AST_arg_t *parse_arg_ast(parser_t *parser);

AST_func_decl_t *parse_func_decl_ast(parser_t *parser);
AST_func_call_t *parse_func_call_ast(parser_t *parser);

AST_scope_t *parse_if_scope_ast(parser_t *parser);
AST_if_stmt_t *parse_if_stmt_ast(parser_t *parser);
#endif