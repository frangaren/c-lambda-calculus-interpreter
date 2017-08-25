#pragma once

#include <stdbool.h>

#include "ast.h"

bool step_in_place (Expression *expression);
void step_eval_in_place (Expression *expression);
