#pragma once

#include "ast.h"
#include "lexer.h"

typedef struct parser {
  Lexer lexer;
} *Parser;

Parser new_parser(Lexer lexer);
Lexer free_parser(Parser *parser);

Expression parse_expression(Parser parser);
