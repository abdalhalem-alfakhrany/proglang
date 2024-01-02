#ifndef _UTILS_
#define _UTILS_
#include <stddef.h>

#include "ast.h"

#define EXPECT_ERROR(expected_token)                                           \
  if (!parser_expect(parser, expected_token, -1)) {                            \
    fprintf(stderr, "%s:%d:%d: error: expect %s here\n\t| %s:%d\n",            \
            "examples/main.prog", parser->current_token->col,                  \
            parser->current_token->row, token_type_to_str(expected_token),     \
            __FILE__, __LINE__);                                               \
    exit(EXIT_FAILURE);                                                        \
  }

#define FAST_TOKEN_ID(name) create_token(TOKEN_ID, name, strlen(name), 0, 0)

#define PRINT_TABS(exta_tabs)                                                  \
  for (int i = 0; i < level + margin + exta_tabs; i++)                         \
    n += sprintf(str + n, "\t");

int read_file(char *filename, size_t *content_size, char **content);
int prec(OP op_typ);
char op_char(OP op);
OP tok_to_op(TOKEN_TYPE tok_type);
char *type_str(ID_TYPE id_type);
int is_operator(int token_type);
ID_TYPE is_type(char *str);

#endif