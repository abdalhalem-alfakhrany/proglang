#include "lexer.h"
#include "token.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

lexer_t *create_lexer(char *src, size_t src_length) {
  lexer_t *lexer = malloc(sizeof(lexer_t));
  lexer->src = src;
  lexer->src_length = src_length;
  lexer->src_index = 0;
  lexer->row = 0;
  lexer->col = 0;
  return lexer;
}

void lexer_trim_left(lexer_t *lexer) {
  while (lexer_current(lexer) == ' ' || lexer_current(lexer) == '\n' ||
         lexer_current(lexer) == '\t') {
    lexer_advance(lexer);
  }
}

void lexer_advance(lexer_t *lexer) {
  if (lexer->src_index < lexer->src_length) {
    lexer->src_index++;
  }
}

char lexer_current(lexer_t *lexer) {
  if (lexer->src_index < lexer->src_length) {
    return lexer->src[lexer->src_index];
  }
  return '\0';
}

token_t *lexer_next_token(lexer_t *lexer) {

  if (lexer_current(lexer) == '\n') {
    lexer->row = 0;
    lexer->col++;
  }
  if (lexer->src_index >= lexer->src_length) {
    return create_token(TOKEN_EOF, "", 0, lexer->col, lexer->row);
  }

  static char buff[256];
  int buff_size = 0;

  lexer_trim_left(lexer);

  memset(buff, '\0', buff_size);
  buff_size = 0;

  if (strchr("(){}:;*-+/=,", lexer_current(lexer)) != NULL) {
    lexer->row++;
    token_t *tok =
        create_token_chr(chr_to_token(lexer_current(lexer)),
                         lexer_current(lexer), lexer->col, lexer->row);
    lexer_advance(lexer);
    return tok;
  }

  if (isalpha(lexer_current(lexer))) {
    while (isalnum(lexer_current(lexer))) {
      buff[buff_size++] = lexer_current(lexer);
      lexer_advance(lexer);
    }
    lexer->row += buff_size;
    return create_token(TOKEN_ID, buff, buff_size, lexer->col, lexer->row);
  }

  if (isdigit(lexer_current(lexer))) {
    while (isdigit(lexer_current(lexer))) {
      buff[buff_size++] = lexer_current(lexer);
      lexer_advance(lexer);
    }
    lexer->row += buff_size;
    return create_token(TOKEN_NUMBER, buff, buff_size, lexer->col, lexer->row);
  }
  return NULL;
}