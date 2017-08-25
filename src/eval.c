#include "eval.h"

#include <stddef.h>

static bool _apply_in_place (Expression *function, Expression *argument,
  int depth);

bool step_in_place (Expression *expression) {
  switch ((*expression)->type) {
    case VARIABLE:
      return false;
      break;
    case LAMBDA:
      return step_in_place(&(*expression)->lambda);
      break;
    case APPLICATION: {
      Expression *function = &(*expression)->application->function;
      Expression *argument = &(*expression)->application->argument;

      if ((*function)->type == LAMBDA) {
        Expression reduced_function =  (*function)->lambda;
        (*function)->lambda = NULL;
        _apply_in_place(&reduced_function, argument, 0);
        free_expression(expression);
        *expression = reduced_function;
        return true;
      } else if (step_in_place(function)) {
        return true;
      } else if (step_in_place(argument)) {
        return true;
      } else {
        return false;
      }
      break;
    }
    default:
      return false;
  }
}

static bool _apply_in_place (Expression *function,
  Expression *argument, int depth)
{
  switch ((*function)->type) {
    case VARIABLE:
      if ((*function)->variable == depth) {
        free_expression(function);
        *function = copy_expression(*argument);
        return true;
      } else {
        return false;
      }
      break;
    case LAMBDA:
      return _apply_in_place(&(*function)->lambda, argument, depth + 1);
      break;
    case APPLICATION: {
      Expression *left = &(*function)->application->function;
      Expression *right = &(*function)->application->argument;
      bool left_outcome = _apply_in_place(left, argument,  depth);
      bool right_outcome = _apply_in_place(right, argument,  depth);
      return left_outcome || right_outcome;
      break;
    }
    default:
      return false;
  }
}
