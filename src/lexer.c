#include "lexer.h"

#include <stdbool.h>
#include <stdlib.h>

#define MAX_NAME_SIZE 33

static Token raw_get_next_token(Lexer lexer);
static wchar_t raw_get_next_char(Lexer lexer);
static wchar_t peek_next_char(Lexer lexer);
static wchar_t get_next_char(Lexer lexer);
static bool is_identifier_char(wchar_t c);
static bool is_space(wchar_t c);

Lexer new_lexer(FILE *stream) {
  if (stream == NULL) return NULL;
  Lexer lexer = malloc(sizeof(struct lexer));
  if (lexer == NULL) return NULL;
  lexer->stream = stream;
  lexer->is_token_buffered = false;
  lexer->is_char_buffered = false;
  return lexer;
}

void free_lexer(Lexer *lexer) {
  free(*lexer);
  *lexer = NULL;
}

static Token raw_get_next_token(Lexer lexer) {
  wchar_t c;
  do {
    c = get_next_char(lexer);
  } while (is_space(c));
  if (c == L'\0') {
    return (Token) {.type = TKN_EOF};
  } else if (c == L'\\' || c == L'λ') {
    return (Token) {.type = TKN_LAMBDA};
  } else if (c == L'.') {
    return (Token) {.type = TKN_DOT};
  } else if (c == L'(') {
    return (Token) {.type = TKN_LPAREN};
  } else if (c == L')') {
    return (Token) {.type = TKN_RPAREN};
  } else {
    wchar_t buffer[MAX_NAME_SIZE] = {c, L'\0'};
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
    buffer[i] = L'\0';
    wchar_t *name = malloc((wcslen(buffer) + 1) * sizeof(wchar_t));
    wcscpy(name, buffer);
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

static wchar_t raw_get_next_char(Lexer lexer) {
  int c = fgetc(lexer->stream);
  if (c == EOF) {
    return L'\0';
  }
  char buffer[5] = {(char)c};
  int length = utf8_byte_type(c);
  if (length <= 0) {
    fprintf(stderr, "Invalid utf-8 char byte.\n");
    return L'\0';
  }
  for (int i = 1; i < length; i++) {
    int temp = fgetc(lexer->stream);
    if (temp == EOF) {
      fprintf(stderr, "Premature EOF while parsing utf-8 character.\n");
      return L'\0';
    }
    buffer[i] = (char)temp;
  }
  wchar_t wc;
  if (mbtowc(&wc, buffer, 5) == -1) {
    fprintf(stderr, "Invalid utf-8 char '%s'.\n", buffer);
    return L'\0';
  }
  return wc;
}

static wchar_t peek_next_char(Lexer lexer) {
  if (!lexer->is_char_buffered) {
    lexer->next_char = raw_get_next_char(lexer);
    lexer->is_char_buffered = true;
  }
  return lexer->next_char;
}

static wchar_t get_next_char(Lexer lexer) {
  if (!lexer->is_char_buffered) {
    lexer->next_char = raw_get_next_char(lexer);
  }
  lexer->is_char_buffered = false;
  return lexer->next_char;
}

static bool is_identifier_char(wchar_t c) {
  switch (c) {
    case L' ':
    case L'\t':
    case L'\r':
    case L'\n':
    case L'\0':
    case L'\\':
    case L'λ':
    case L'.':
    case L'(':
    case L')':
      return false;
      break;
    default:
      return true;
  }
}

static bool is_space(wchar_t c) {
  switch (c) {
    case L' ':
    case L'\t':
    case L'\r':
    case L'\n':
      return true;
      break;
    default:
      return false;
  }
}
