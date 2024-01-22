#include "../src/lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void lexer_should_lex() {

  token_t tokens[4] = {
      {.token_type = TOKEN_NUMBER, .text = "1", .col = 1, .row = 1},
      {.token_type = OP_PLUS, .text = "+", .col = 3, .row = 1},
      {.token_type = TOKEN_NUMBER, .text = "1", .col = 5, .row = 1},
      {.token_type = TOKEN_SIMI, .text = ";", .col = 6, .row = 1},
  };
  char *src = "1 + 1;";

  lexer_t *lexer = create_lexer(src, strlen(src));

  token_t *token;
  for (int i = 0; i < 4; i++) {
    token = lexer_next_token(lexer);
    fprintf(stderr, "---%d---\n", i);
    if (token->token_type != tokens[i].token_type) {
      fprintf(stderr, "type not the same\n");
    }
    if (strcmp(token->text, tokens[i].text)) {
      fprintf(stderr, "text not the same\n");
    }
    if (token->col != tokens[i].col) {
      fprintf(stderr, "col not the same %d != %d\n", token->col, tokens[i].col);
    }
    if (token->row != tokens[i].row) {
      fprintf(stderr, "row not the same %d != %d\n", token->row, tokens[i].row);
    }
    fprintf(stderr, "------\n\n", i);
  }
  fprintf(stderr, "good job\n");
}

int main(int argc, char const *argv[]) {
  lexer_should_lex();
  return 0;
}
