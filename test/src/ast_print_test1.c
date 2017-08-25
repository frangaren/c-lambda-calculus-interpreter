#include <stdio.h>

#define AST_SHORT
#include "ast.h"

int main(int argc, char **argv) {
  // Original expression: (λ n f x. f (n f x)) (λ f x. f x)
  // Expected output: (λa.λb.λc.b (a b c)) (λa.λb.a b)
  Expression e = app(
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
  printf("\t\t");
  print_expression(e);
  printf("\n");
  free_expression(&e);
  return 0;
}
