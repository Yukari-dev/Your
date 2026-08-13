#include "../include/AST.h"
#include <stdlib.h>

ASTNode *CreateNumberNode(int number){
  ASTNode *node = malloc(sizeof(ASTNode));
  if(!node)
    return NULL;
  node->type= NT_NUMBER;
  node->NodeData.number = number;
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

void FreeASTNode(ASTNode *node){
  if (!node)
    return;
  if (node->type == NT_BINARY_OP){
    FreeASTNode(node->NodeData.nodeOperation.left);
    FreeASTNode(node->NodeData.nodeOperation.right);
  }
  free(node);
}
