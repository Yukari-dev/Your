#ifndef AST_H
#define AST_H

typedef enum{
  NT_NUMBER,
  NT_STRING,
  NT_BINARY_OP,
  NT_VAR_DECL,
  NT_ASSIGNMENT,
  NT_IDENTIFIER,
} NodeType;

typedef struct{
  char op;
  struct s_ASTNode *left;
  struct s_ASTNode *right;
} NodeOperation;

typedef struct{
  char name[64];
  char typeName[32];
  struct s_ASTNode *value;
} VarDecl;

typedef struct{
  char name[64];
  struct s_ASTNode *value;
} Assignment;

typedef struct s_ASTNode{
  NodeType type;
  union{
    int number;
    char string[128];
    char identifier[64];
    NodeOperation nodeOperation;
    VarDecl varDecl;
    Assignment assignment;
  } NodeData;
} ASTNode;

ASTNode *CreateNumberNode(int number);

ASTNode *CreateStringNode(char *string);

ASTNode *CreateIdentifierNode(char *identifier);

ASTNode *CreateAssignmentNode(char *assignment, ASTNode *val);

ASTNode *CreateBinaryNode(char op, ASTNode *left, ASTNode *right);

ASTNode *CreateVarDeclNode(char *name, char *typeName, ASTNode *value);

void FreeASTNode(ASTNode *node);

#endif
