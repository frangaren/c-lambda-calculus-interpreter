#pragma once

#include <stdint.h>

typedef uint64_t Variable;

typedef struct expression *Lambda;

typedef struct application {
  struct expression *function;
  struct expression *argument;
} *Application;

typedef enum expression_type {
  VARIABLE,
  LAMBDA,
  APPLICATION
} ExpressionType;

typedef struct expression {
  ExpressionType type;
  union {
    Variable variable;
    Lambda lambda;
    Application application;
  };
} *Expression;

Expression variable(uint64_t bruijin_index);
Expression lambda(Expression expression);
Expression application(Expression function, Expression argument);

#ifdef AST_SHORT
#define var(x)    (variable((x)))
#define lam(x)    (lambda((x)))
#define app(f, x) (application((f),(x)))
#endif

void free_expression(Expression *expression);
void print_expression(Expression expression);
