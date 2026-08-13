#include "../include/parser.h"
#include "../include/helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Parser *InitParser(const char *source) {
  Parser *parser = malloc(sizeof(Parser));
  if (!parser)
    return NULL;
  parser->lexer = InitLexer(source);
  parser->table = InitSymbolTable(64);

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
  if(parser->current_token.tokenType == TT_IDENTIFIER){
    char str[128];
    strncpy(str, parser->current_token.buffer, parser->current_token.length);
    str[parser->current_token.length] = '\0';
    ASTNode *node = CreateIdentifierNode(str);
    AdvanceParser(parser);
    return node;
  }
  if (parser->current_token.tokenType == TT_STRING_LITERAL){
    char str[128];
    strncpy(str, parser->current_token.buffer, parser->current_token.length);
    str[parser->current_token.length] = '\0';
    ASTNode *node = CreateStringNode(str);
    AdvanceParser(parser);
    return node;
  }
  printf("Syntax Error: Unexpected factor token '%s'\n", parser->current_token.buffer);
  AdvanceParser(parser);
  return NULL;
}

ASTNode *ParseTerm(Parser *parser) {
  ASTNode *left = ParseFactor(parser);

  while (parser->current_token.tokenType == TT_ASTERISK || parser->current_token.tokenType == TT_SLASH) {
    char op = parser->current_token.buffer[0];
    AdvanceParser(parser);
    ASTNode *right = ParseFactor(parser);
    left = CreateBinaryNode(op, left, right);
  }
  return left;
}

ASTNode *ParseExpression(Parser *parser) {
  ASTNode *left = ParseTerm(parser);
  while (parser->current_token.tokenType == TT_ADDITION || parser->current_token.tokenType == TT_SUBTRACT) {
    char op = parser->current_token.buffer[0];
    AdvanceParser(parser);
    ASTNode *right = ParseTerm(parser);
    left = CreateBinaryNode(op, left, right);
  }
  return left;
}

ASTNode *ParseVarDecl(Parser *parser){
  AdvanceParser(parser);
  if(parser->current_token.tokenType != TT_IDENTIFIER){
    printf("Syntax Error: Expected variable name after 'var'\n");
    return NULL;
  }
  char name[64];
  strncpy(name, parser->current_token.buffer, parser->current_token.length);
  name[parser->current_token.length] = '\0';

  AdvanceParser(parser);
  if(parser->current_token.tokenType != TT_GREATER){
    printf("Syntax Error: Expected '>' after variable name '%s'\n", name);
    return NULL;
  }

  AdvanceParser(parser);
  if(parser->current_token.tokenType != TT_VAR_TYPE){
    printf("Syntax Error: Expected type after '>'\n");
    return NULL;
  }
  char type[32];
  strncpy(type, parser->current_token.buffer, parser->current_token.length);
  type[parser->current_token.length] = '\0';

  AdvanceParser(parser);
  if(parser->current_token.tokenType != TT_EQUAL){
    printf("Syntax Error: Expected '=' after type '%s'\n", type);
    return NULL;
  }

  AdvanceParser(parser);
  ASTNode *initializer = ParseExpression(parser);
  if(!initializer){
    printf("Syntax Error: Expected valid expression after '='\n");
    return NULL;
  }
  return CreateVarDeclNode(name, type, initializer);
}

ASTNode *ParseStatement(Parser *parser){
  if(parser->current_token.tokenType == TT_VAR){
    return ParseVarDecl(parser);
  }
  if(parser->current_token.tokenType == TT_IDENTIFIER && parser->peak_token.tokenType == TT_EQUAL){
    char str[64];
    strncpy(str, parser->current_token.buffer, parser->current_token.length);
    str[parser->current_token.length] = '\0';

    AdvanceParser(parser);
    AdvanceParser(parser);
  
    ASTNode *rhs = ParseExpression(parser);
    return CreateAssignmentNode(str, rhs);
  }
  return ParseExpression(parser);
}

ASTNode *ParseProgram(Parser *parser){
  while (parser->current_token.tokenType != TT_EOF){
    ASTNode *stmt = ParseStatement(parser);

    if(stmt){
      EvaluateAST(stmt, parser->table);
      // PrintSymbolTable(parser->table);
      FreeASTNode(stmt);
    }
  }
  return NULL;
}

Value EvaluateAST(ASTNode *node, SymbolTable *table) {
  if (node == NULL) return MakeNull();
  if (node->type == NT_NUMBER) return MakeNumber(node->NodeData.number);
  if (node->type == NT_STRING) return MakeString(node->NodeData.string);

  if (node->type == NT_VAR_DECL){
    Value val = EvaluateAST(node->NodeData.varDecl.value, table);
    if (!VariableComparison(node->NodeData.varDecl.typeName, ValueTypeToString(val.valueType))){
      printf("Type Error: Cannot assign [%s] to [%s] variable [%s]\n", 
              node->NodeData.varDecl.typeName, 
              ValueTypeToString(val.valueType),
              node->NodeData.varDecl.name
      );
      return MakeNull();
    }
    SetSymbol(table, node->NodeData.varDecl.name, val);
    return val;
  }
  if (node->type == NT_IDENTIFIER){
    Value val;
    if(GetSymbol(table, node->NodeData.identifier, &val)){
      return val;
    }
    printf("Runtime Error: Variable '%s' is not defined!\n", node->NodeData.identifier);
    return MakeNull();
  }

  if(node->type == NT_ASSIGNMENT){
    Value dummy;
    if(!GetSymbol(table, node->NodeData.assignment.name, &dummy)){
      printf("Runtime Error: Cannot assign to undefined variable '%s'!\n", node->NodeData.assignment.name);
      return MakeNull();
    }
    Value val = EvaluateAST(node->NodeData.assignment.value, table);
    if (!VariableComparison(ValueTypeToString(dummy.valueType), ValueTypeToString(val.valueType))){
      printf("Type Error: Cannot assign [%s] to [%s] variable [%s]\n", 
              ValueTypeToString(dummy.valueType), 
              ValueTypeToString(val.valueType),
              node->NodeData.varDecl.name
      );
      return MakeNull();
    }
    SetSymbol(table, node->NodeData.assignment.name, val);
    return val;
  }

  if(node->type == NT_BINARY_OP){
    Value left = EvaluateAST(node->NodeData.nodeOperation.left, table);
    Value right = EvaluateAST(node->NodeData.nodeOperation.right, table);
    char op = node->NodeData.nodeOperation.op;
    if (op == '+') return MakeNumber(left.as.as_number + right.as.as_number);
    else if (op == '-') return MakeNumber(left.as.as_number - right.as.as_number);
    else if (op == '*') return MakeNumber(left.as.as_number * right.as.as_number);
    else if (op == '/') {
      if (right.as.as_number == 0) {
        printf("Error: Division by zero!\n");
        return MakeNull();
      }
      return MakeNumber(left.as.as_number / right.as.as_number);
    }
  }

  return MakeNull();
}
