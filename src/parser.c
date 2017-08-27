#include "parser.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/**********
* GRAMMAR *
***********

Program     ::= Expression EOF
Expression  ::= Lambda
              | Variable
              | Application
              | "(" Expression ")"
Lambda      ::= "λ" Name "." Expression
Variable    ::= Name
Name        ::= [^ \nλ.=()]+
Application ::= Function Argument
Function    ::= Variable
              | Application
              | "(" Expression ")"
Argument    ::= Lambda
              | Variable
              | "(" Expression ")"

*/

static Expression parse_expression(Parser parser);
static Expression parse_lambda(Parser parser);
static Expression parse_variable(Parser parser);
//static Expression parse_application(Parser parser);
//static Expression parse_function(Parser parser);
//static Expression parse_argument(Parser parser);
static Expression parse_parenthesis(Parser parser);

Parser new_parser(Lexer lexer) {
  if (lexer == NULL) return NULL;
  Parser output = malloc(sizeof(struct parser));
  if (output == NULL) return NULL;
  output->lexer = lexer;
  output->resolver = new_variable_resolver();
  return output;
}

Lexer free_parser(Parser *parser) {
  Lexer lexer = (*parser)->lexer;
  free_variable_resolver(&(*parser)->resolver);
  free(*parser);
  *parser = NULL;
  return lexer;
}

Expression parse(Parser parser) {
  Expression expression = parse_expression(parser);
  Token token = get_next_token(parser->lexer);
  if (token.type != TKN_EOF) {
    free_expression(&expression);
    free_token(token);
    fprintf(stderr, "%s: Error, end of file expected.\n", __func__);
    exit(-2);
  }
  free_token(token);
  return expression;
}

Expression parse_expression(Parser parser) {
  if (peek_next_token(parser->lexer).type == TKN_LAMBDA) {
    // Lambda rule
    return parse_lambda(parser);
  } else if (peek_next_token(parser->lexer).type == TKN_NAME) {
    // Variable
    return parse_variable(parser);
  } else if (peek_next_token(parser->lexer).type == TKN_LPAREN) {
    // Parenthesis
    return parse_parenthesis(parser);
  } else {
    return NULL;
  }
}

static Expression parse_lambda(Parser parser) {
  // <lambda><name><dot><expression>
  Token token = get_next_token(parser->lexer);
  if (token.type != TKN_LAMBDA) {
    free_token(token);
    fprintf(stderr, "%s: Error, lambda(\\) expected.\n", __func__);
    exit(-2);
  }
  free_token(token);
  token = get_next_token(parser->lexer);
  if (token.type != TKN_NAME) {
    free_token(token);
    fprintf(stderr, "%s: Error, name expected.\n", __func__);
    exit(-2);
  }
  if (!add_variable(&parser->resolver, token.name)) {
    free_token(token);
    fprintf(stderr, "%s: Couldn't add variable '%s'. Not enough memory.\n",
      __func__, token.name);
    exit(2);
  }
  free_token(token);
  token = get_next_token(parser->lexer);
  if (token.type != TKN_DOT) {
    free_token(token);
    fprintf(stderr, "%s: Error, dot(.) expected.\n", __func__);
    exit(-2);
  }
  free_token(token);
  Expression expression = parse_expression(parser);
  return lambda(expression);
}

static Expression parse_variable(Parser parser) {
  //<name>
  Token token = get_next_token(parser->lexer);
  if (token.type != TKN_NAME) {
    free_token(token);
    fprintf(stderr, "%s: Error, name expected.\n", __func__);
    exit(-2);
  }
  uint64_t index;
  if (!get_variable_bruijin(&parser->resolver, token.name, &index)) {
    free_token(token);
    fprintf(stderr, "%s: Couldn't find '%s'. Variable not in scope.\n",
      __func__, token.name);
    exit(-2);
  }
  free_token(token);
  return variable(index);
}

static Expression parse_parenthesis(Parser parser) {
  // <lparen><expression><rparen>
  Token token = get_next_token(parser->lexer);
  if (token.type != TKN_LPAREN) {
    free_token(token);
    fprintf(stderr, "%s: Error, left parenthesis('(') expected.\n", __func__);
    exit(-2);
  }
  if (!add_variable_scope(&parser->resolver)) {
    free_token(token);
    fprintf(stderr, "%s: Couldn't create a new scope.\n", __func__);
    exit(-2);
  }
  free_token(token);
  Expression expression = parse_expression(parser);
  token = get_next_token(parser->lexer);
  if (token.type != TKN_RPAREN) {
    free_expression(&expression);
    free_token(token);
    fprintf(stderr, "%s: Error, right parenthesis(')') expected.\n", __func__);
    exit(-2);
  }
  if (!drop_variable_scope(&parser->resolver)) {
    free_expression(&expression);
    free_token(token);
    fprintf(stderr, "%s: Couldn't drop a scope.\n",__func__);
    exit(-2);
  }
  free_token(token);
  return expression;
}
