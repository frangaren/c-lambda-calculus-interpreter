#pragma once

#include <stdbool.h>

#include "ast.h"
#include "variable_resolver.h"

void eval(Statement statement, VariableResolver *resolver);

bool step_expression_in_place (Expression *expression,
  VariableResolver *resolver);
void eval_expression_in_place (Expression *expression,
  VariableResolver *resolver);
void print_eval_expression_in_place (Expression *expression,
  VariableResolver *resolver);
