#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"

int read_file(char *filename, size_t *content_size, char **file_content) {
  FILE *file = fopen(filename, "r");

  if (file == NULL) {
    return -1;
  }

  fseek(file, 0, SEEK_END);
  *content_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  *file_content = malloc((sizeof(char) * *content_size) + 1);
  fread(*file_content, *content_size, 1, file);

  return fclose(file);
}

int prec(OP op_typ) {
  switch (op_typ) {
  case OP_MINUS:
  case OP_PLUS:
    return 1;
  case OP_MULT:
  case OP_DIV:
    return 2;
  default:
    return 0;
  }
}

char op_char(OP op) {
  switch (op) {
  case OP_PLUS:
    return '+';
  case OP_MINUS:
    return '-';
  case OP_MULT:
    return '*';
  case OP_DIV:
    return '/';

  default:
    printf("`%c` operator is unknown\n", op);
    return '\0';
  }
}

int is_operator(int token_type) {
  return token_type == OP_PLUS || token_type == OP_MINUS ||
         token_type == OP_MULT || token_type == OP_DIV;
}

char *type_str(ID_TYPE id_type) {
  switch (id_type) {
  case ID_TYPE_INT:
    return "int";
  case ID_TYPE_FLOAT:
    return "float";

  default:
    return "unknown type";
  }
}

ID_TYPE is_type(char *str) {
  if (!strcmp(str, "int"))
    return ID_TYPE_INT;
  if (!strcmp(str, "float"))
    return ID_TYPE_FLOAT;
  return ID_TYPE_UNKNOWN;
}