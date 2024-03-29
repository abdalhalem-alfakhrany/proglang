SRC=src/main.c src/utils.c  \
	src/lexer.c src/token.c \
 	src/parser.c src/linked_list.c \
  	src/ast.c src/stack.c	\
	src/evaluator.c 

FLAGS=-Wall -Wextra -Wswitch

proglang:
	gcc $(SRC) -DDEBUG_AST_TERMINAL -o build/proglang
	
proglang.debug:
	gcc $(SRC) $(FLAGS) -DDEBUG -ggdb -o build/proglang

test.lex:
	gcc src/utils.c src/lexer.c src/token.c test/lexer.c -ggdb -o lex_test