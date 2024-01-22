#include <ctype.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "evaluator.h"
#include "lexer.h"
#include "linked_list.h"
#include "parser.h"
#include "stack.h"
#include "token.h"
#include "utils.h"

#include "debug/ast_debug.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage : ./proglang <input.prog> options\n");
    exit(EXIT_FAILURE);
  }
  char *filename = argv[1];
  char *src;
  size_t src_length;
  if (read_file(filename, &src_length, &src)) {
    perror("read_file ");
    exit(EXIT_FAILURE);
  }

  lexer_t *lexer = create_lexer(src, src_length);
  parser_t *parser = create_parser(lexer);
  // scope_context_t *program_context = create_context(NULL);
  AST_scope_t *program = parse_program_ast(parser);

  // eval_scope(program_context, program);

  // int error_code = eval_func_call(program_context,
  //                                 create_ast_func_call(FAST_TOKEN_ID("main")))
  //                      ->return_value;
  // printf("return is %d\n", error_code);

  free_ast_scope(program);
  free_parser(parser);
  free_lexer(lexer);

  return 0;
}