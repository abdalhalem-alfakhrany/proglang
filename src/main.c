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
  scope_context_t *program_context = create_context(NULL);

  AST_scope_t *program = parse_program_ast(parser);

  if (argc > 3) {
    if (!strcmp(argv[2], "-ast_dump")) {
      if (!strcmp(argv[3], "graph")) {
        FILE *out_file = fopen("build/tree.dot", "w");
        print_ast_scope(out_file, program, 0, DUMP_TYPE_GRAPHVIZ);
        fclose(out_file);
      } else if (!strcmp(argv[3], "term")) {
        print_ast_scope(stdout, program, 0, DUMP_TYPE_TERM);
      }
    }
  }

  eval_scope(program_context, program);

  int error_code = eval_func_call(program_context,
                                  create_ast_func_call(FAST_TOKEN_ID("main")))
                       ->return_value;
  printf("return is %d\n", error_code);

  free_ast_scope(program);

  return 0;
}