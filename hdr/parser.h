#pragma once

#include "ast.h"
#include "lexer.h"
#include "variable_resolver.h"

typedef struct parser {
  Lexer lexer;
  VariableResolver resolver;
} *Parser;

Parser new_parser(Lexer lexer);
Lexer free_parser(Parser *parser);

Expression parse(Parser parser);
