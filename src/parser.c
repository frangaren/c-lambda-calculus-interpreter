#include "parser.h"

#include <stdlib.h>

Parser new_parser(Lexer lexer) {
  if (lexer == NULL) return NULL;
  Parser output = malloc(sizeof(struct parser));
  if (output == NULL) return NULL;
  output->lexer = lexer;
  return output;
}

Lexer free_parser(Parser *parser) {
  Lexer lexer = (*parser)->lexer;
  free(*parser);
  *parser = NULL;
  return lexer;
}

Expression parse_expression(Parser parser) {
  Token token = get_next_token(parser->lexer);
  while (token.type != TKN_EOF) {
    free_token(token);
    token = get_next_token(parser->lexer);
  }
  free_token(token);
  return NULL;
}
