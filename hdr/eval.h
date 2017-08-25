#pragma once

#include <stdbool.h>

#include "ast.h"

bool step_in_place (Expression *expression);
void eval_in_place (Expression *expression);
void print_eval_in_place (Expression *expression);
