#include <assert.h>
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
  Expression outcome = copy_expression(e);
  step_in_place(&e);
  assert(check_equal_expression(e, outcome));
  free_expression(&e);
  free_expression(&outcome);
  return 0;
}
