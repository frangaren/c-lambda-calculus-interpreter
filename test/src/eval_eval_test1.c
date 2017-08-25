#include <assert.h>
#include <stdio.h>

#define AST_SHORT
#include "ast.h"
#include "eval.h"

int main(int argc, char **argv) {
  /* Expected output:
        (λa.λb.λc.b (a b c)) (λa.λb.a b) ⟶
          λa.λb.a (a b)
  */
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
  Expression outcome = lam(lam(app(
                                   var(1),
                                   app(
                                       var(1),
                                       var(0)))));
  eval_in_place(&e);
  assert(check_equal_expression(e, outcome));
  free_expression(&e);
  free_expression(&outcome);
  return 0;
}
