#ifndef _TOKEN_
#define _TOKEN_
#include <stdint.h>

typedef enum TOKEN_TYPE_E TOKEN_TYPE;
enum TOKEN_TYPE_E {
  TOKEN_EMPTY = 0,
  TOKEN_RET,
  TOKEN_COLON,
  TOKEN_ID,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_LPRACE,
  TOKEN_RPRACE,
  TOKEN_NUMBER,
  TOKEN_SIMI,
  TOKEN_EQUAL,
  TOKEN_COMMA,
  TOKEN_IF,
  TOKEN_ELSE,
  TOKEN_EOF,
};

typedef enum OP_E OP;
enum OP_E {
  OP_PLUS = TOKEN_EOF + 1,
  OP_MINUS,
  OP_MULT,
  OP_DIV,
};

int chr_to_token(char chr);

char *token_type_to_str(int type);

typedef struct token token_t;
struct token {
  int token_type;
  int col, row;
  char *text;
};

token_t *create_token_chr(TOKEN_TYPE type, char chr, int col, int row);
token_t *create_token(TOKEN_TYPE type, char *text, int token_len, int col,
                      int row);
void print_token(token_t *token);
void sprint_token(char *str, token_t *token);
#endif