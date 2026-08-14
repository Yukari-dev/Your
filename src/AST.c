#include "../include/AST.h"
#include "../include/SymbolTable.h"
#include <stdlib.h>
#include <string.h>

ASTNode *CreateNumberNode(int number){
  ASTNode *node = malloc(sizeof(ASTNode));
  if(!node)
    return NULL;
  node->type= NT_NUMBER;
  node->NodeData.number = number;
  return node;
}

ASTNode *CreateStringNode(char *string){
  ASTNode *node = malloc(sizeof(ASTNode));
  if(!node)
    return NULL;
  node->type = NT_STRING;
  strcpy(node->NodeData.string, string);
  node->NodeData.string[127] = '\0';
  return node;
}

ASTNode *CreateIdentifierNode(char *identifier){
  ASTNode *node = malloc(sizeof(ASTNode));
  if(!node)
    return NULL;
  node->type = NT_IDENTIFIER;
  strcpy(node->NodeData.identifier, identifier);
  node->NodeData.identifier[63] = '\0';
  return node;
}

ASTNode *CreateAssignmentNode(char *assignment, ASTNode *val){
  ASTNode *node = malloc(sizeof(ASTNode));
  if(!node)
    return NULL;
  node->type = NT_ASSIGNMENT;
  strcpy(node->NodeData.assignment.name, assignment);
  node->NodeData.assignment.name[63] = '\0';
  node->NodeData.assignment.value = val;
  return node;
}

ASTNode *CreateBinaryNode(char op, ASTNode *left, ASTNode *right){
  ASTNode *node = malloc(sizeof(ASTNode));
  if(!node)
    return NULL;
  node->type = NT_BINARY_OP;
  node->NodeData.nodeOperation.op = op;
  node->NodeData.nodeOperation.left = left;
  node->NodeData.nodeOperation.right = right;
  return node;
}

ASTNode *CreateVarDeclNode(char *name, char *typeName, ASTNode *value){
  ASTNode *node = malloc(sizeof(ASTNode));
  if (!node)
    return NULL;
  node->type = NT_VAR_DECL;
  strcpy(node->NodeData.varDecl.name, name);
  strcpy(node->NodeData.varDecl.typeName, typeName);
  node->NodeData.varDecl.value = value;

  return node;
}

ASTNode *CreateFnDeclNode(char *name, char *giveType, Parameter *params, size_t paramCount, ASTNode **body, size_t stmtCount){
  ASTNode *node = malloc(sizeof(ASTNode));
  if (!node)
    return NULL;
  node->type = NT_FN_DECL;
  strncpy(node->NodeData.fnDecl.name, name, 64);
  strncpy(node->NodeData.fnDecl.giveType, giveType, 32);
  node->NodeData.fnDecl.paramCount = paramCount;
  node->NodeData.fnDecl.statementCount = stmtCount;

  for(size_t i = 0; i < paramCount; i++)
    node->NodeData.fnDecl.params[i] = params[i];
  for(size_t i = 0; i < stmtCount; i++)
    node->NodeData.fnDecl.body[i] = body[i];
  return node;
}

ASTNode *CreateGiveNode(ASTNode *giveNode){
  ASTNode *node = malloc(sizeof(ASTNode));
  if (!node)
    return NULL;
  node->type = NT_GIVE;
  node->NodeData.giveStatement.value = giveNode;
  return node;
}

ASTNode *CreateFnCallNode(char *name, ASTNode **args, size_t argCount){
  ASTNode *node = malloc(sizeof(ASTNode));
  if (!node)
    return NULL;
  node->type = NT_FN_CALL;
  strncpy(node->NodeData.fnCall.name, name, 64);
  node->NodeData.fnCall.argCount = argCount;
  for(size_t i = 0; i < argCount; i++)
    node->NodeData.fnCall.args[i] = args[i];
  return node;
}

ASTNode *CreateWhenNode(ASTNode *condition, ASTNode **body, size_t statementCount){
  ASTNode *node = malloc(sizeof(ASTNode));
  if (!node)
    return NULL;
  node->type = NT_WHEN;
  node->NodeData.when.statementCount = statementCount;
  node->NodeData.when.condition = condition; 
  node->NodeData.when.body = malloc(sizeof(ASTNode*) * statementCount);
  for(size_t i = 0; i < statementCount; i++)
    node->NodeData.when.body[i] = body[i];
  return node;
}

void FreeASTNode(ASTNode *node){
  if (!node)
    return;
  if (node->type == NT_BINARY_OP){
    FreeASTNode(node->NodeData.nodeOperation.left);
    FreeASTNode(node->NodeData.nodeOperation.right);
  }
  free(node);
}
