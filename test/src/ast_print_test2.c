#include <stdio.h>

#define AST_SHORT
#include "ast.h"

int main(int argc, char **argv) {
  // Original expression: (λ x. x x) (λ x. x x)
  // Expected output: (λa.a a) (λa.a a)
  Expression e = app(
                     lam(app(
                             var(0),
                             var(0))),
                     lam(app(
                             var(0),
                             var(0))));
  printf("\t\t");
  print_expression(e);
  printf("\n");
  free_expression(&e);
  return 0;
}
