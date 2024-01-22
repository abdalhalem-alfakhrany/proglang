#ifndef _LEXER_
#define _LEXER_
#include "token.h"
#include <stddef.h>

typedef struct lexer lexer_t;
struct lexer {
  char *src;
  size_t src_index;
  size_t src_length;
  int row, col;
};

lexer_t *create_lexer(char *src, size_t src_length);
void free_lexer(lexer_t *lexer);
void lexer_trim_left(lexer_t *lexer);
void lexer_advance(lexer_t *lexer);
char lexer_current(lexer_t *lexer);
token_t *lexer_next_token(lexer_t *lexer);

#endif
