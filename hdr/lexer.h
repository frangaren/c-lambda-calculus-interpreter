#pragma once

#include <stdio.h>

#include "token.h"

typedef struct lexer {
  FILE *stream;
  int next_char;
} *Lexer;

Lexer new_lexer(FILE *stream);
void free_lexer(Lexer *lexer);

Token get_next_token(Lexer lexer);
