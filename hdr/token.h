#pragma once

#include <wchar.h>

typedef enum token_type {
  TKN_LAMBDA,
  TKN_NAME,
  TKN_DOT,
  TKN_LPAREN,
  TKN_RPAREN,
  TKN_ASSIGN,
  TKN_LET,
  TKN_SEPARATOR,
  TKN_EOF
} TokenType;

typedef struct token {
  TokenType type;
  union {
    wchar_t *name;
  };
} Token;

void free_token(Token token);

void print_token(Token token);
