#ifdef EXE
#include "lexer.h"
#include "token.h"

int main(int argc, char **argv) {
  Lexer lexer = new_lexer(stdin);
  Token token = get_next_token(lexer);
  while (token.type != TKN_EOF) {
    print_token(token);
    free_token(token);
    token = get_next_token(lexer);
  }
  print_token(token);
  free_token(token);
  free_lexer(&lexer);
  return 0;
}

#endif
