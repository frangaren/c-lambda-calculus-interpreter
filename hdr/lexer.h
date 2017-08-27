#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "token.h"

typedef struct lexer {
  FILE *stream;
  int next_char;
  Token next_token;
  bool is_token_buffered;
} *Lexer;

Lexer new_lexer(FILE *stream);
void free_lexer(Lexer *lexer);

Token get_next_token(Lexer lexer);
Token peek_next_token(Lexer lexer);
