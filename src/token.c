#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *token_type_to_str(int type) {
  switch (type) {

  case TOKEN_ID:
    return "ID";
    break;
  case TOKEN_LPAREN:
    return "(";
    break;
  case TOKEN_RPAREN:
    return ")";
    break;
  case TOKEN_LPRACE:
    return "{";
    break;
  case TOKEN_RPRACE:
    return "}";
    break;
  case TOKEN_NUMBER:
    return "Number";
    break;
  case TOKEN_SIMI:
    return ";";
    break;
  case TOKEN_COLON:
    return ":";
    break;
  case TOKEN_EMPTY:
    return "";
    break;
  case TOKEN_EOF:
    return "EOF";
    break;
  case OP_PLUS:
    return "+";
    break;
  case OP_MINUS:
    return "-";
    break;
  case OP_MULT:
    return "*";
    break;
  case OP_DIV:
    return "/";
    break;
  case TOKEN_EQUAL:
    return "=";
    break;
  case TOKEN_IF:
    return "IF";
    break;
  case TOKEN_ELSE:
    return "ELSE";
    break;
  default:
    return "undefined type";
    break;
  }
}

token_t *create_token(TOKEN_TYPE type, char *text, int token_len, int col,
                      int row) {
  token_t *token = malloc(sizeof(token_t));
  token->token_type = type;
  token->text = malloc(sizeof(char) * token_len);
  strncpy(token->text, text, token_len);
  token->text[token_len] = '\0';
  token->row = row;
  token->col = col;
  return token;
}

token_t *create_token_chr(TOKEN_TYPE type, char chr, int col, int row) {
  token_t *token = malloc(sizeof(token_t));
  token->token_type = type;
  token->text = malloc(sizeof(char) * 2);
  token->text[0] = chr;
  token->text[1] = '\0';
  token->row = row;
  token->col = col;
  return token;
}

void free_token(token_t *token) {
  if (token->text != NULL)
    free(token->text);
  free(token);
}

int chr_to_token(char chr) {
  switch (chr) {
  case ' ':
    return TOKEN_EMPTY;
  case '+':
    return OP_PLUS;
  case '-':
    return OP_MINUS;
  case '*':
    return OP_MULT;
  case '/':
    return OP_DIV;
  case ':':
    return TOKEN_COLON;
  case '(':
    return TOKEN_LPAREN;
  case ')':
    return TOKEN_RPAREN;
  case '{':
    return TOKEN_LPRACE;
  case '}':
    return TOKEN_RPRACE;
  case ';':
    return TOKEN_SIMI;
  case '=':
    return TOKEN_EQUAL;
  case ',':
    return TOKEN_COMMA;

  default:
    return TOKEN_EMPTY;
  }
}

void print_token(token_t *token) {
  char buff[64];
  sprint_token(buff, token);
  printf("%s\n", buff);
}

void sprint_token(char *str, token_t *token) {
  if (token->token_type == TOKEN_EMPTY)
    sprintf(str, "<empty token>");
  else
    sprintf(str, "<token %s %s>", token->text,
            token_type_to_str(token->token_type));
}