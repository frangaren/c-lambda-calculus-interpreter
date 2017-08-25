#include <assert.h>

#define AST_SHORT
#include "ast.h"

int main(int argc, char **argv) {
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
  assert(check_equal_expression(e1, e2));
  free_expression(&e1);
  free_expression(&e2);
  return 0;
}
