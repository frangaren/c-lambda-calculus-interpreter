#ifdef EXE

#include <locale.h>
#include <stdio.h>

#include "ast.h"
#include "eval.h"
#include "lexer.h"
#include "parser.h"
#include "token.h"
#include "variable_resolver.h"

int main(int argc, char **argv) {
  setlocale(LC_CTYPE, "en_US.utf8");
  if (argc != 2) {
    fprintf(stderr, "\tExpected one argument. %d given.\n", argc - 1);
    return 2;
  }
  FILE *input = fopen(argv[1], "r");
  Lexer lexer = new_lexer(input);
  Parser parser = new_parser(lexer);
  VariableResolver resolver = new_variable_resolver();
  Statement s = parse(parser);
  while (s != NULL) {
    eval(s, &resolver);
    print_statement(s);
    printf("\n");
    free_statement(&s);
    s = parse(parser);
  }
  free_variable_resolver(&resolver);
  free_parser(&parser);
  free_lexer(&lexer);
  fclose(input);
  return 0;
}

#endif
