#include <stdio.h>

#define AST_SHORT
#include "ast.h"

int main(int argc, char **argv) {
  // Original expression:  λz. (λy. y (λx. x)) (λx. z x)
  // Expected output: λa.(λb.b (λc.c)) (λb.a b)
  Expression e = lam(app(
                         lam(app(
                                 var(0),
                                 lam(var(0)))),
                         lam(app(
                                 var(1),
                                 var(0)))));
  printf("\t\t");
  print_expression(e);
  printf("\n");
  free_expression(&e);
  return 0;
}
