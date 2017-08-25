#include <stdio.h>

#define AST_SHORT
#include "ast.h"
#include "eval.h"

int main(int argc, char **argv) {
  /* Expected output:
        (λa.a a) (λa.a a) ⟶
          (λa.a a) (λa.a a)
  */
  Expression e = app(
                     lam(app(
                             var(0),
                             var(0))),
                     lam(app(
                             var(0),
                             var(0))));
  printf("\t\t");
  print_expression(e);
  printf(" ⟶\n");
  step_in_place(&e);
  printf("\t\t\t");
  print_expression(e);
  printf("\n");
  free_expression(&e);
  return 0;
}
