#ifndef AST_H
#define AST_H

typedef enum{
  NT_NUMBER,
  NT_BINARY_OP,
} NodeType;

typedef struct{
  char op;
  struct s_ASTNode *left;
  struct s_ASTNode *right;
} NodeOperation;

typedef struct s_ASTNode{
  NodeType type;
  union{
    int number;
    NodeOperation nodeOperation;
  } NodeData;
} ASTNode;

ASTNode *CreateNumberNode(int number);

ASTNode *CreateBinaryNode(char op, ASTNode *left, ASTNode *right);

void FreeASTNode(ASTNode *node);

#endif
