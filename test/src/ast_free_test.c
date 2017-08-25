#include <stdlib.h>

#include "ast.h"

int main (int argc, char **argv) {
  free_expression(NULL);
  Expression e = NULL;
  free_expression(&e);
  return 0;
}
