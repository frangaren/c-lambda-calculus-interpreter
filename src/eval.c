#include "eval.h"

#include <stddef.h>
#include <stdio.h>
#include <wctype.h>

static bool _apply_in_place (Expression *function, Expression *argument,
  int depth);
static bool _rebase(Expression argument, int rebase_depth, int depth);

void eval(Statement statement, VariableResolver *resolver) {
  if (statement == NULL) return;
  switch (statement->type) {
    case EXPRESSION:
      eval_expression_in_place(&statement->expression, resolver);
      break;
    case LET:
      if (!add_variable(resolver, statement->let->binding,
                        copy_expression(statement->let->expression))) {
        fprintf(stderr, "%s: Couldn't add variable '%S'. Not enough memory.\n",
                __func__, statement->let->binding);
      }
      break;
  }
}

bool step_expression_in_place (Expression *expression,
  VariableResolver *resolver) {
  if (expression == NULL || *expression == NULL) return false;
  switch ((*expression)->type) {
    case VARIABLE:
      return false;
      break;
    case LAMBDA:
      return step_expression_in_place(&(*expression)->lambda, resolver);
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
      } else if (step_expression_in_place(function, resolver)) {
        return true;
      } else if (step_expression_in_place(argument, resolver)) {
        return true;
      } else {
        return false;
      }
      break;
    }
    case GLOBAL: {
      Expression binding = NULL;
      if (get_variable_data(resolver, (*expression)->global, &binding)) {
        Expression new_expression = copy_expression(binding);
        free_expression(expression);
        *expression = new_expression;
        return true;
      } else {
        return false;
      }
      break;
    }
  }
  return false;
}

static bool _apply_in_place (Expression *function,
  Expression *argument, int depth)
{
  switch ((*function)->type) {
    case VARIABLE:
      if ((*function)->variable == depth) {
        free_expression(function);
        *function = copy_expression(*argument);
        _rebase(*function, depth, 0);
        return true;
      } else if ((*function)->variable > depth) {
        (*function)->variable--;
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
    case GLOBAL:
      return false;
      break;
  }
  return false;
}

static bool _rebase(Expression expression, int rebase_depth, int depth) {
  if (rebase_depth == 0) return false;
  switch (expression->type) {
    case VARIABLE:
      if (expression->variable >= depth) {
        expression->variable += rebase_depth;
        return true;
      } else {
        return false;
      }
      break;
    case LAMBDA:
      return _rebase(expression->lambda, rebase_depth, depth + 1);
      break;
    case APPLICATION: {
      bool left = _rebase(expression->application->function, rebase_depth,
        depth);
      bool right = _rebase(expression->application->argument, rebase_depth,
        depth);
      return left || right;
      break;
    }
    case GLOBAL:
      return false;
      break;
  }
  return false;
}

void eval_expression_in_place (Expression *expression,
  VariableResolver *resolver) {
  while (step_expression_in_place(expression, resolver));
}

void print_eval_expression_in_place (Expression *expression,
  VariableResolver *resolver) {
  if (expression == NULL || *expression == NULL) {
    printf("\n");
    return;
  }
  print_expression(*expression);
  bool change = false;
  do {
    change = step_expression_in_place(expression, resolver);
    if (change) {
      printf(" ⟶\n\t");
      print_expression(*expression);
    }
  } while (change);
  printf("\n");
}
