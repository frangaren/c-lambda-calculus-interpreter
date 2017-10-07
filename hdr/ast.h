#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

typedef uint64_t Variable;

typedef struct expression *Lambda;

typedef struct application {
  struct expression *function;
  struct expression *argument;
} *Application;

typedef wchar_t *Global;

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

typedef struct let {
  Global binding;
  Expression expression;
} *Let;

typedef enum statement_type {
  EXPRESSION,
  LET
} StatementType;

typedef struct statement {
  StatementType type;
  union {
    Expression expression;
    Let let;
  };
} *Statement;

Expression variable(uint64_t bruijin_index);
Expression lambda(Expression expression);
Expression application(Expression function, Expression argument);
Expression global(wchar_t *name);
Statement expression(Expression expression);
Statement let(Global binding, Expression expression);

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

void print_statement(Statement statement);

bool check_equal_statement(Statement a, Statement b);
Statement copy_statement(Statement statement);
void free_statement(Statement *statement);
