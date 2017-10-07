#ifdef EXE

#include <locale.h>
#include <stdio.h>

#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "eval.h"

int main(int argc, char **argv) {
  setlocale(LC_CTYPE, "en_US.utf8");
  if (argc != 2) {
    fprintf(stderr, "\tExpected one argument. %d given.\n", argc - 1);
    return 2;
  }
  FILE *input = fopen(argv[1], "r");
  Lexer lexer = new_lexer(input);
  Parser parser = new_parser(lexer);
  Expression e = parse(parser);
  while (e != NULL) {
    eval_in_place(&e);
    print_expression(e);
    printf("\n");
    free_expression(&e);
    e = parse(parser);
  }
  free_parser(&parser);
  free_lexer(&lexer);
  fclose(input);
  return 0;
}

#endif
