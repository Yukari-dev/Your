#ifndef PARSER_H
#define PARSER_H
#include "AST.h"
#include "lexer.h"
#include <stdbool.h>

typedef struct {
  Lexer *lexer;
  Token current_token;
  Token peak_token;
} Parser;

Parser *InitParser(Lexer *lexer);

void AdvanceParser(Parser *parser);

void ExpectToken(Parser *parser, TokenType tokenType);

ASTNode *ParseFactor(Parser *parser);

ASTNode *ParseTerm(Parser *parser);

ASTNode *ParseExpression(Parser *parser);

#endif
