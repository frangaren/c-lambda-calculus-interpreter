#ifdef EXE

#include <stdio.h>

#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "eval.h"

int main(int argc, char **argv) {
  printf(">> ");
  Lexer lexer = new_lexer(stdin);
  Parser parser = new_parser(lexer);
  Expression e = parse(parser);
  printf("\nOutput:\n");
  eval_in_place(&e);
  print_expression(e);
  printf("\n");
  free_expression(&e);
  free_parser(&parser);
  free_lexer(&lexer);
  return 0;
}

#endif
