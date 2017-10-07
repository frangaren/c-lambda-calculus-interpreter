#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <uchar.h>
#include <wchar.h>

#include "token.h"
#include "utf8.h"

typedef struct lexer {
  FILE *stream;
  Token next_token;
  bool is_token_buffered;
  wchar_t next_char;
  bool is_char_buffered;
} *Lexer;

Lexer new_lexer(FILE *stream);
void free_lexer(Lexer *lexer);

Token get_next_token(Lexer lexer);
Token peek_next_token(Lexer lexer);
