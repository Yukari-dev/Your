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
  parser->table = InitSymbolTable();

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
  if(parser->current_token.tokenType == TT_OPEN_PAREN){
    AdvanceParser(parser);
    ASTNode *expr = ParseExpression(parser);
    if(parser->current_token.tokenType == TT_CLOSE_PAREN)
      AdvanceParser(parser);
    else
      printf("Syntax Error: Expected ')'\n");
    return expr;
  }
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
    char varName[64];
    strncpy(varName, parser->current_token.buffer, parser->current_token.length);
    varName[parser->current_token.length] = '\0';
    if(parser->peak_token.tokenType == TT_COLON){
      char fnName[64];
      strncpy(fnName, parser->current_token.buffer, parser->current_token.length);
      fnName[parser->current_token.length] = '\0';
      AdvanceParser(parser);
      ASTNode *args[16];
      size_t argCount = 0;
      AdvanceParser(parser);
      ASTNode *firstArg = ParseExpression(parser);
      if(firstArg) args[argCount++] = firstArg;
      else return NULL;
      while (parser->current_token.tokenType == TT_COMMA){
        AdvanceParser(parser);
        ASTNode *argExpr = ParseExpression(parser);
        if(argExpr) args[argCount++] = argExpr;
        else break;
      }
      return CreateFnCallNode(fnName, args, argCount);
    }
    ASTNode *node = CreateIdentifierNode(varName);
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
  printf("Syntax Error: Unexpected factor token '%.*s'\n",  (int)parser->current_token.length, parser->current_token.buffer);
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

ASTNode *ParseFnDecl(Parser *parser){
  AdvanceParser(parser);
  if(parser->current_token.tokenType != TT_IDENTIFIER){
    printf("Syntax Error: Expected function name after 'fn'\n");
    return NULL;
  }
  char name[64];
  strncpy(name, parser->current_token.buffer, parser->current_token.length);
  name[parser->current_token.length] = '\0';
  
  AdvanceParser(parser);
  char giveType[32] = "void";
  if (parser->current_token.tokenType == TT_GREATER) {
    AdvanceParser(parser);
    if (parser->current_token.tokenType == TT_VAR_TYPE) {
      strncpy(giveType, parser->current_token.buffer, parser->current_token.length);
      giveType[parser->current_token.length] = '\0';
      AdvanceParser(parser);
    }
  }

  Parameter params[16];
  size_t paramCount = 0;
  if (parser->current_token.tokenType == TT_COLON){
    AdvanceParser(parser);
    while (parser->current_token.tokenType == TT_IDENTIFIER){
      strncpy(params[paramCount].name, parser->current_token.buffer, parser->current_token.length);
      params[paramCount].name[parser->current_token.length] = '\0';
      AdvanceParser(parser);
      if(parser->current_token.tokenType != TT_GREATER){
        printf("Syntax Error: Expected '>' to specify the type of '%s'.\n", params[paramCount].name);
        return NULL;
      }
      AdvanceParser(parser);
      if (parser->current_token.tokenType != TT_VAR_TYPE){
        printf("Syntax Error: Invalid variable type '%s' of variable '%s'.\n", 
                TokenTypeToString(parser->current_token.tokenType), params[paramCount].name
        );
        return NULL;
      }
      strncpy(params[paramCount].typeName, parser->current_token.buffer, parser->current_token.length);
      params[paramCount].typeName[parser->current_token.length] = '\0';
      AdvanceParser(parser);
      paramCount++;
      if(parser->current_token.tokenType == TT_COMMA) AdvanceParser(parser);
      else break;
    }
  }

  if(parser->current_token.tokenType != TT_OPEN_SCOPE){
    printf("Syntax Error: Expected '~>' to begin function block\n");
    return NULL;
  }

  AdvanceParser(parser);
  ASTNode *body[128];
  size_t stmtCount = 0;

  while (parser->current_token.tokenType != TT_CLOSE_SCOPE && parser->current_token.tokenType != TT_EOF){
    if (parser->current_token.tokenType == TT_GIVE){
      AdvanceParser(parser);
      body[stmtCount++] = CreateGiveNode(ParseExpression(parser));
      continue;
    }
    ASTNode *stmt = ParseStatement(parser);
    if(stmt) body[stmtCount++] = stmt;
  }

  if (parser->current_token.tokenType == TT_CLOSE_SCOPE) {
    AdvanceParser(parser);
  } else {
    printf("Syntax Error: Expected '<~' to end function block\n");
    return NULL;
  }
  return CreateFnDeclNode(name, giveType, params, paramCount, body, stmtCount);
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
  if(parser->current_token.tokenType == TT_FN_DECL){
    return ParseFnDecl(parser);
  }
  return ParseExpression(parser);
}

ASTNode *ParseProgram(Parser *parser){
  while (parser->current_token.tokenType != TT_EOF){
    ASTNode *stmt = ParseStatement(parser);

    if(stmt){
      EvaluateAST(stmt, parser->table);
      // PrintSymbolTable(parser->table);
      if (stmt->type != NT_FN_DECL) {
        FreeASTNode(stmt);
      }
    }
  }
  return NULL;
}

Value ExecuteFn(ASTNode *fnNode, Value *args, size_t argCount, SymbolTable *globalTable){
  SymbolTable *localTable = InitSymbolTable();
  localTable->parent = globalTable; 

  for(size_t i = 0; i < fnNode->NodeData.fnDecl.paramCount; i++)
    SetSymbol(localTable, fnNode->NodeData.fnDecl.params[i].name, args[i]);

  Value giveValue = MakeNull();
  
  for(size_t i = 0; i < fnNode->NodeData.fnDecl.statementCount; i++){
    ASTNode *stmt = fnNode->NodeData.fnDecl.body[i];

    if(stmt->type == NT_GIVE){
      giveValue = EvaluateAST(stmt->NodeData.giveStatement.value, localTable);
      break;
    }
    EvaluateAST(stmt, localTable);
  }
  FreeSymbolTable(localTable);
  return giveValue;
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
              node->NodeData.assignment.name 
      );
      return MakeNull();
    }
    SetSymbol(table, node->NodeData.assignment.name, val);
    return val;
  }

  if(node->type == NT_FN_DECL){
    Value fnVal;
    fnVal.valueType = VT_FN;
    fnVal.as.as_ast = node;

    SetSymbol(table, node->NodeData.fnDecl.name, fnVal);
    return MakeNull();
  }

  if(node->type == NT_FN_CALL){
    if(strcmp(node->NodeData.fnCall.name, "reveal") == 0){
      if(node->NodeData.fnCall.argCount == 0){
        printf("Runtime Error: There are no arguments to reveal.\n");
        return MakeNull();
      }
      ASTNode *fmtNode = node->NodeData.fnCall.args[0];
      Value fmtVal = EvaluateAST(fmtNode, table);
      if(fmtVal.valueType != VT_STRING){
        printf("First argument to reveal must be a format string\n");
        return MakeNull();
      }
      const char *str = fmtVal.as.as_string;
      size_t argIndex = 1;
      for(size_t i = 0; i < strlen(str); i++){
        if (str[i] == '%'){
          i++;
          Value val = EvaluateAST(node->NodeData.fnCall.args[argIndex], table);
          if(str[i] == 's'){
            argIndex++;
            printf("%s", val.as.as_string);
          }
          if(str[i] == 'd'){
            argIndex++;
            printf("%d", (int)val.as.as_number);
          }
          if(str[i] == 'g'){
            argIndex++;
            printf("%g", val.as.as_number);
          }
          continue;
        }
        printf("%c", str[i]);
      }

      printf("\n");
      return MakeNull();
    }
    Value fnVal;
    if(!GetSymbol(table, node->NodeData.fnCall.name, &fnVal)){
      printf("Runtime Error: Undefined function '%s'\n", node->NodeData.fnCall.name);
      return MakeNull();
    }
    Value args[16];
    for(size_t i = 0; i < node->NodeData.fnCall.argCount; i++)
      args[i] = EvaluateAST(node->NodeData.fnCall.args[i], table);
    return ExecuteFn(fnVal.as.as_ast, args, node->NodeData.fnCall.argCount, table); 
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
