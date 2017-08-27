#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef uint64_t Variable;

typedef struct expression *Lambda;

typedef struct application {
  struct expression *function;
  struct expression *argument;
} *Application;

typedef char *Global;

typedef enum expression_type {
  VARIABLE,
  LAMBDA,
  APPLICATION,
  GLOBAL
} ExpressionType;

typedef struct expression {
  ExpressionType type;
  union {
    Variable variable;
    Lambda lambda;
    Application application;
    Global global;
  };
} *Expression;

Expression variable(uint64_t bruijin_index);
Expression lambda(Expression expression);
Expression application(Expression function, Expression argument);
Expression global(char *name);

#ifdef AST_SHORT
#define var(x)    (variable((x)))
#define lam(x)    (lambda((x)))
#define app(f, x) (application((f),(x)))
#define glo(x)    (global((x)))
#endif

void print_expression(Expression expression);

bool check_equal_expression(Expression a, Expression b);
Expression copy_expression(Expression expression);
void free_expression(Expression *expression);
