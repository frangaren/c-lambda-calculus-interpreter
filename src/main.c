#ifdef EXE

#include "parser.h"
#include "lexer.h"
#include "token.h"
#include "ast.h"
#include "eval.h"

int main(int argc, char **argv) {
  Lexer lexer = new_lexer(stdin);
  Parser parser = new_parser(lexer);
  Expression e = parse_expression(parser);
  print_eval_in_place(&e);
  free_expression(&e);
  free_parser(&parser);
  free_lexer(&lexer);
  return 0;
}

#endif
