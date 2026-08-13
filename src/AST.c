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

void FreeASTNode(ASTNode *node){
  if (!node)
    return;
  if (node->type == NT_BINARY_OP){
    FreeASTNode(node->NodeData.nodeOperation.left);
    FreeASTNode(node->NodeData.nodeOperation.right);
  }
  free(node);
}
