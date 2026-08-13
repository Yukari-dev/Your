#include "../include/parser.h"
#include "../include/helper.h"
#include <stdlib.h>

Parser *InitParser(Lexer *lexer) {
  Parser *parser = malloc(sizeof(Parser));
  if (!parser)
    return NULL;
  parser->lexer = lexer;

  AdvanceParser(parser);
  AdvanceParser(parser);

  return parser;
}

void AdvanceParser(Parser *parser) {
  parser->current_token = parser->peak_token;
  parser->peak_token = GetNextToken(parser->lexer);
}

void ExpectToken(Parser *parser, TokenType tokenType) {
  if (parser->current_token.tokenType == tokenType)
    AdvanceParser(parser);
  else
    printf("[ERROR: [%d:%d]]", parser->lexer->line, parser->lexer->column);
}

ASTNode *ParseFactor(Parser *parser) {
  if (parser->current_token.tokenType == TT_NUMBER) {
    int x = 0;
    for (size_t i = 0; i < parser->current_token.length; i++) {
      x = (x * 10) + (parser->current_token.buffer[i] - '0');
    }
    ASTNode *node = CreateNumberNode(x);
    AdvanceParser(parser);
    return node;
  }
  printf("Expected number\n");
  return NULL;
}

ASTNode *ParseTerm(Parser *parser) {
  ASTNode *left = ParseFactor(parser);

  while (parser->current_token.tokenType == TT_ASTERISK || parser->current_token.tokenType == TT_SLASH){
    char op = parser->current_token.buffer[0];
    AdvanceParser(parser);
    ASTNode *right = ParseFactor(parser);
    left = CreateBinaryNode(op, left, right);
  }
  return left;
}

ASTNode *ParseExpression(Parser *parser){
  ASTNode *left = ParseTerm(parser);
  while (parser->current_token.tokenType == TT_ADDITION || parser->current_token.tokenType == TT_SUBTRACT){
    char op = parser->current_token.buffer[0];
    AdvanceParser(parser);
    ASTNode *right = ParseTerm(parser);
    left = CreateBinaryNode(op, left, right);
  }
  return left;
}
