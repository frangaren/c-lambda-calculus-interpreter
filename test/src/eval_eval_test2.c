#include <assert.h>
#include <stdio.h>

#define AST_SHORT
#include "ast.h"
#include "eval.h"

int main(int argc, char **argv) {
  /* Expected output:
        λa.λb.(λc.b c) a ⟶
          λa.λb.b a
  */
  Expression e = lam(lam(app(
                             lam(app(
                                     var(1),
                                     var(0))),
                             var(1))));
  Expression outcome = lam(lam(app(
                                   var(0),
                                   var(1))));
  eval_expression_in_place(&e, NULL);
  assert(check_equal_expression(e, outcome));
  free_expression(&e);
  free_expression(&outcome);
  return 0;
}
