#include "token.h"

#include <stdio.h>
#include <stdlib.h>

void free_token(Token token) {
  switch (token.type) {
    case TKN_NAME:
      free(token.name);
      break;
    default:
      break;
  }
}

void print_token(Token token) {
  switch (token.type) {
    case TKN_NAME:
      fprintf(stderr, "<name(\"%s\")>", token.name);
      break;
    case TKN_EOF:
      fprintf(stderr, "<eof>");
      break;
    case TKN_DOT:
      fprintf(stderr, "<.>");
      break;
    case TKN_LAMBDA:
      fprintf(stderr, "<λ>");
      break;
    case TKN_LPAREN:
      fprintf(stderr, "<(>");
      break;
    case TKN_RPAREN:
      fprintf(stderr, "<)>");
      break;
    default:
      fprintf(stderr, "<INVALID TOKEN>");
      break;
  }
}
