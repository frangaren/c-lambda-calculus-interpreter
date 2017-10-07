#include "ast.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

static void _print_expression(Expression expression, const char *letters,
  int current, int max);

Expression variable(uint64_t bruijin_index) {
  Expression outcome = malloc(sizeof(struct expression));
  if (outcome == NULL) {
    return NULL;
  } else {
    *outcome = (struct expression) {.type = VARIABLE,
                                    .variable = bruijin_index};
    return outcome;
  }
}

Expression lambda(Expression expression) {
  if (expression == NULL) return NULL;
  Expression outcome = malloc(sizeof(struct expression));
  if (outcome == NULL) {
    return NULL;
  } else {
    *outcome = (struct expression) {.type = LAMBDA,
                                    .lambda = expression};
    return outcome;
  }
}

Expression application(Expression function, Expression argument) {
  if (function == NULL || argument == NULL) return NULL;
  Expression outcome = malloc(sizeof(struct expression));
  if (outcome == NULL) {
    return NULL;
  } else {
    Application application = malloc(sizeof(struct application));
    if (application == NULL) {
      free(outcome);
      return NULL;
    } else {
      *application = (struct application) {.function = function,
                                           .argument = argument};
      *outcome = (struct expression) {.type = APPLICATION,
                                      .application = application};
      return outcome;
    }
  }
}

Expression global(wchar_t *name) {
  if (name == NULL) return NULL;
  Expression outcome = malloc(sizeof(struct expression));
  if (outcome == NULL) {
    return NULL;
  } else {
    *outcome = (struct expression) {.type = GLOBAL};
    outcome->global = malloc((wcslen(name) + 1) * sizeof(wchar_t));
    if (outcome->global == NULL) {
      free(outcome);
      return NULL;
    } else {
      wcscpy(outcome->global, name);
      return outcome;
    }
  }
}

Statement expression(Expression expression) {
  if (expression == NULL) return NULL;
  Statement outcome = malloc(sizeof(struct statement));
  if (outcome == NULL) {
    return NULL;
  } else {
    *outcome = (struct statement) {.type = EXPRESSION,
                                   .expression = expression};
    return outcome;
  }
}

Statement let(Global binding, Expression expression) {
  if (expression == NULL || binding == NULL) return NULL;
  Statement outcome = malloc(sizeof(struct statement));
  if (outcome == NULL) {
    return NULL;
  } else {
    Let let = malloc(sizeof(struct let));
    if (let == NULL) {
      free(outcome);
      return NULL;
    } else {
      let->binding = malloc((wcslen(binding) + 1)*sizeof(wchar_t));
      if (let->binding == NULL) {
        return NULL;
      } else {
        wcscpy(let->binding, binding);
        let->expression = expression;
        *outcome = (struct statement) {.type = LET,
                                       .let = let};
        return outcome;
      }
    }
  }
}

void print_expression(Expression expression) {
  if (expression == NULL) return;
  const char letters[] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  _print_expression(expression, letters, 0, sizeof(letters)/sizeof(char) - 1);
}

static void _print_expression(Expression expression, const char *letters,
  int current, int max)
{
  if (current >= max) {
    fprintf(stderr, "%s: No variables left.\n", __func__);
    exit(2);
  }
  switch (expression->type) {
    case VARIABLE: {
      int i = current - expression->variable - 1;
      if (i < 0) {
        fprintf(stderr, "%s: Invalid variable index %"PRIu64".\n", __func__,\
          expression->variable);
        exit(2);
      } else {
        printf("%c", letters[i]);
      }
      break;
    }
    case LAMBDA: {
      printf("λ%c.", letters[current]);
      _print_expression(expression->lambda, letters, current + 1, max);
      break;
    }
    case APPLICATION: {
      Expression function = expression->application->function;
      Expression argument = expression->application->argument;
      switch (function->type) {
        case GLOBAL:
        case VARIABLE:
        case APPLICATION:
          _print_expression(function, letters, current, max);
          break;
        case LAMBDA:
          printf("(");
          _print_expression(function, letters, current, max);
          printf(")");
      }
      printf(" ");
      switch (argument->type) {
        case GLOBAL:
        case VARIABLE:
          _print_expression(argument, letters, current, max);
          break;
        case LAMBDA:
        case APPLICATION:
          printf("(");
          _print_expression(argument, letters, current, max);
          printf(")");
      }
      break;
    }
    case GLOBAL: {
      printf("%S", expression->global);
      break;
    }
    default: {
      fprintf(stderr, "%s: Invalid expression type %d.\n", __func__,\
        expression->type);
      exit(2);
    }
  }
}

bool check_equal_expression(Expression a, Expression b) {
  if (a == NULL || b == NULL) return false;
  if (a->type == b->type) {
    switch (a->type) {
      case VARIABLE:
        return a->variable == b->variable;
        break;
      case LAMBDA:
        return check_equal_expression(a->lambda, b->lambda);
        break;
      case APPLICATION: {
        Expression af = a->application->function;
        Expression ax = a->application->argument;
        Expression bf = b->application->function;
        Expression bx = b->application->argument;
        return check_equal_expression(af, bf) &&\
          check_equal_expression(ax, bx);
        break;
      }
      case GLOBAL:
        return wcscmp(a->global, b->global) == 0;
    }
    return false;
  } else {
    return false;
  }
}

void free_expression(Expression *expression) {
  if (expression == NULL || *expression == NULL) return;
  switch ((*expression)->type) {
    case LAMBDA:
      free_expression(&(*expression)->lambda);
      break;
    case APPLICATION:
      free_expression(&(*expression)->application->function);
      free_expression(&(*expression)->application->argument);
      free((*expression)->application);
      break;
    case GLOBAL:
      free((*expression)->global);
    case VARIABLE:
      break;
  }
  free(*expression);
  *expression = NULL;
}

Expression copy_expression(Expression expression) {
  Expression outcome = NULL;
  switch (expression->type) {
    case VARIABLE:
      outcome = variable(expression->variable);
      break;
    case LAMBDA: {
      Expression lambda_expression = copy_expression(expression->lambda);
      if (lambda_expression == NULL) return NULL;
      outcome = lambda(lambda_expression);
      if (outcome == NULL) {
        free_expression(&lambda_expression);
        return NULL;
      }
      break;
    }
    case APPLICATION: {
      Expression function = copy_expression(expression->application->function);
      if (function == NULL) return NULL;
      Expression argument = copy_expression(expression->application->argument);
      if (argument == NULL) {
        free_expression(&argument);
        return NULL;
      }
      outcome = application(function, argument);
      if (outcome == NULL) {
        free_expression(&function);
        free_expression(&argument);
        return NULL;
      }
      break;
    }
    case GLOBAL: {
      return global(expression->global);
    }
  }
  return outcome;
}

void print_statement(Statement statement) {
  if (statement == NULL) return;
  switch (statement->type) {
    case EXPRESSION:
      print_expression(statement->expression);
      break;
    case LET:
      printf("let %S := ", statement->let->binding);
      print_expression(statement->let->expression);
      break;
    default:
      fprintf(stderr, "%s: Error, undefinded behaviour.\n", __func__);
  }
  printf(";");
}

bool check_equal_statement(Statement a, Statement b) {
  if (a == NULL || b == NULL) return false;
  if (a->type == b->type) {
    switch (a->type) {
      case EXPRESSION:
        return check_equal_expression(a->expression, b->expression);
        break;
      case LET:
        if (wcscmp(a->let->binding, b->let->binding) != 0) {
          return false;
        }
        return check_equal_expression(a->let->expression, b->let->expression);
    }
    return false;
  } else {
    return false;
  }
}

Statement copy_statement(Statement statement) {
  if (statement != NULL) {
    switch (statement->type) {
      case EXPRESSION:
        return expression(copy_expression(statement->expression));
        break;
      case LET:
        return let(statement->let->binding,
                   copy_expression(statement->let->expression));
        break;
    }
  }
  return NULL;
}

void free_statement(Statement *statement) {
  if (statement == NULL || *statement == NULL) return;
  switch ((*statement)->type) {
    case EXPRESSION:
      free_expression(&(*statement)->expression);
      break;
    case LET:
      free((*statement)->let->binding);
      (*statement)->let->binding = NULL;
      free_expression(&(*statement)->let->expression);
      break;
  }
  free(*statement);
  *statement = NULL;
}
