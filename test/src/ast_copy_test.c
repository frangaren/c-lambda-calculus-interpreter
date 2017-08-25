#include <stdio.h>

#define AST_SHORT
#include "ast.h"

int main(int argc, char **argv) {
  /* Expected output:
        AST1: (λa.λb.λc.b (a b c)) (λa.λb.a b)
        AST2: (λa.λb.λc.b (a b c)) (λa.λb.a b)
  */
  Expression e1 = app(
                      lam(lam(lam(app(
                                      var(1),
                                      app(
                                          app(
                                              var(2),
                                              var(1)),
                                          var(0)))))),
                      lam(lam(app(
                                  var(1),
                                  var(0)))));
  Expression e2 = copy_expression(e1);
  printf("\t\tAST1: ");
  print_expression(e1);
  printf("\n");
  free_expression(&e1);
  printf("\t\tAST2: ");
  print_expression(e2);
  printf("\n");
  free_expression(&e2);
  return 0;
}
