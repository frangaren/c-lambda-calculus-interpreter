#include "parser.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "queue.h"

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
Argument    ::= Variable
              | "(" Expression ")"

*/

static Expression parse_expression(Parser parser);
static Expression parse_lambda(Parser parser);
static Expression parse_variable(Parser parser);
static Expression parse_possible_application(Parser parser);
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
  if (expression == NULL) return NULL;
  Token token = get_next_token(parser->lexer);
  if (token.type != TKN_EOF) {
    free_expression(&expression);
    free_token(token);
    fprintf(stderr, "%s: Error, end of file expected.\n", __func__);
    return NULL;
  }
  free_token(token);
  return expression;
}

Expression parse_expression(Parser parser) {
  if (peek_next_token(parser->lexer).type == TKN_LAMBDA) {
    // Lambda rule
    return parse_lambda(parser);
  } else if (peek_next_token(parser->lexer).type == TKN_NAME ||
             peek_next_token(parser->lexer).type == TKN_LPAREN) {
    // Parenthesis
    return parse_possible_application(parser);
  } else {
    fprintf(stderr, "%s: Error, lambda(\\), name or left parenthesis"\
                    "('(') expected.\n", __func__);
    return NULL;
  }
}

static Expression parse_lambda(Parser parser) {
  // <lambda><name><dot><expression>
  Token token = get_next_token(parser->lexer);
  if (token.type != TKN_LAMBDA) {
    free_token(token);
    fprintf(stderr, "%s: Error, lambda(\\) expected.\n", __func__);
    return NULL;
  }
  free_token(token);
  token = get_next_token(parser->lexer);
  if (token.type != TKN_NAME) {
    free_token(token);
    fprintf(stderr, "%s: Error, name expected.\n", __func__);
    return NULL;
  }
  if (!add_variable(&parser->resolver, token.name)) {
    free_token(token);
    fprintf(stderr, "%s: Couldn't add variable '%s'. Not enough memory.\n",
      __func__, token.name);
    return NULL;
  }
  free_token(token);
  token = get_next_token(parser->lexer);
  if (token.type != TKN_DOT) {
    free_token(token);
    fprintf(stderr, "%s: Error, dot(.) expected.\n", __func__);
    return NULL;
  }
  free_token(token);
  Expression expression = parse_expression(parser);
  if (expression == NULL) return NULL;
  return lambda(expression);
}

static Expression parse_variable(Parser parser) {
  //<name>
  Token token = get_next_token(parser->lexer);
  if (token.type != TKN_NAME) {
    free_token(token);
    fprintf(stderr, "%s: Error, name expected.\n", __func__);
    return NULL;
  }
  uint64_t index;
  Expression expression = NULL;
  if (!get_variable_bruijin(&parser->resolver, token.name, &index)) {
    expression = global(token.name);
  } else {
    expression = variable(index);
  }
  free_token(token);
  if (expression == NULL) return NULL;
  return expression;
}

static Expression parse_parenthesis(Parser parser) {
  // <lparen><expression><rparen>
  Token token = get_next_token(parser->lexer);
  if (token.type != TKN_LPAREN) {
    free_token(token);
    fprintf(stderr, "%s: Error, left parenthesis('(') expected.\n", __func__);
    return NULL;
  }
  if (!add_variable_scope(&parser->resolver)) {
    free_token(token);
    fprintf(stderr, "%s: Couldn't create a new scope.\n", __func__);
    return NULL;
  }
  free_token(token);
  Expression expression = parse_expression(parser);
  if (expression == NULL) return NULL;
  token = get_next_token(parser->lexer);
  if (token.type != TKN_RPAREN) {
    free_expression(&expression);
    free_token(token);
    fprintf(stderr, "%s: Error, right parenthesis(')') expected.\n", __func__);
    return NULL;
  }
  if (!drop_variable_scope(&parser->resolver)) {
    free_expression(&expression);
    free_token(token);
    fprintf(stderr, "%s: Couldn't drop a scope.\n",__func__);
    return NULL;
  }
  free_token(token);
  return expression;
}

static Expression parse_possible_application(Parser parser) {
  Queue queue = new_queue();
  while (1) {
    if (peek_next_token(parser->lexer).type == TKN_NAME) {
      Expression expression = parse_variable(parser);
      if (!push_to_queue(queue, expression)) {
        free_expression(&expression);
        while (!is_empty_queue(queue)){
          Expression to_delete = pop_from_queue(queue);
          free_expression(&to_delete);
        }
        free_queue(&queue);
        fprintf(stderr, "%s: Couldn't push to queue.\n", __func__);
        return NULL;
      }
    } else if (peek_next_token(parser->lexer).type == TKN_LPAREN) {
      Expression expression = parse_parenthesis(parser);
      if (!push_to_queue(queue, expression)) {
        free_expression(&expression);
        while (!is_empty_queue(queue)){
          Expression to_delete = pop_from_queue(queue);
          free_expression(&to_delete);
        }
        free_queue(&queue);
        fprintf(stderr, "%s: Couldn't push to queue.\n", __func__);
        return NULL;
      }
    } else {
      break;
    }
  }
  if (queue->length == 0) {
    fprintf(stderr, "%s: Application expected.\n", __func__);
    return NULL;
  } else if (queue->length == 1) {
    Expression expression = pop_from_queue(queue);
    free_queue(&queue);
    return expression;
  } else {
    Expression accumulator = pop_from_queue(queue);
    Expression x;
    while (!is_empty_queue(queue)) {
      x = pop_from_queue(queue);
      accumulator = application(accumulator, x);
    }
    free_queue(&queue);
    return accumulator;
  }
}
