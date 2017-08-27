#include "lexer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_SIZE 33

static Token raw_get_next_token(Lexer lexer);
static int peek_next_char(Lexer lexer);
static int get_next_char(Lexer lexer);
static bool is_identifier_char(int c);
static bool is_space(int c);

Lexer new_lexer(FILE *stream) {
  if (stream == NULL) return NULL;
  Lexer lexer = malloc(sizeof(struct lexer));
  if (lexer == NULL) return NULL;
  lexer->stream = stream;
  return lexer;
}

void free_lexer(Lexer *lexer) {
  free(*lexer);
  *lexer = NULL;
}

static Token raw_get_next_token(Lexer lexer) {
  int c;
  do {
    c = get_next_char(lexer);
  } while (is_space(c));
  if (c == EOF) {
    return (Token) {.type = TKN_EOF};
  } else if (c == '\\') {
    return (Token) {.type = TKN_LAMBDA};
  } else if (c == '.') {
    return (Token) {.type = TKN_DOT};
  } else if (c == '(') {
    return (Token) {.type = TKN_LPAREN};
  } else if (c == ')') {
    return (Token) {.type = TKN_RPAREN};
  } else {
    char buffer[MAX_NAME_SIZE] = {(char)c, '\0'};
    int i = 1;
    while (is_identifier_char(peek_next_char(lexer))) {
      if (i >= MAX_NAME_SIZE) {
        fprintf(stderr, "%s: Maximum identifier size is %d characters.",
          __func__, MAX_NAME_SIZE - 1);
        exit(2);
      }
      buffer[i] = get_next_char(lexer);
      i++;
    }
    buffer[i] = '\0';
    char *name = malloc((strlen(buffer) + 1) * sizeof(char));
    strcpy(name, buffer);
    return (Token) {.type = TKN_NAME, .name = name};
  }
}

Token get_next_token(Lexer lexer) {
  if (lexer->is_token_buffered) {
    lexer->is_token_buffered = false;
    return lexer->next_token;
  } else {
    return raw_get_next_token(lexer);
  }
}

Token peek_next_token(Lexer lexer) {
  if (lexer->is_token_buffered) {
    return lexer->next_token;
  } else {
    lexer->is_token_buffered = true;
    lexer->next_token = raw_get_next_token(lexer);
    return lexer->next_token;
  }
}

static int peek_next_char(Lexer lexer) {
  int c = fgetc(lexer->stream);
  ungetc(c, lexer->stream);
  return c;
}

static int get_next_char(Lexer lexer) {
  return fgetc(lexer->stream);
}

static bool is_identifier_char(int c) {
  switch (c) {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
    case EOF:
    case '\\':
    case '.':
    case '(':
    case ')':
      return false;
      break;
    default:
      return true;
  }
}

static bool is_space(int c) {
  switch (c) {
    case ' ':
    case '\t':
    case '\r':
    case '\n':
      return true;
      break;
    default:
      return false;
  }
}
