#ifndef PARSER_H
#define PARSER_H
#include "AST.h"
#include "SymbolTable.h"
#include "lexer.h"
#include <stdbool.h>

typedef struct {
  Lexer *lexer;
  Token current_token;
  Token peak_token;
  SymbolTable *table;
} Parser;

Parser *InitParser(const char *source);

void AdvanceParser(Parser *parser);

void ExpectToken(Parser *parser, TokenType tokenType);

ASTNode *ParseFactor(Parser *parser);
ASTNode *ParseTerm(Parser *parser);
ASTNode *ParseExpression(Parser *parser);
ASTNode *ParseVarDecl(Parser *parser);
ASTNode *ParseStatement(Parser *parser);
ASTNode *ParseProgram(Parser *parser);

Value EvaluateAST(ASTNode *node, SymbolTable *table);

#endif
