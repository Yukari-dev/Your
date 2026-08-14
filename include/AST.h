#ifndef AST_H
#define AST_H

#include <stddef.h>
typedef enum {
  NT_NUMBER,
  NT_STRING,
  NT_BINARY_OP,
  NT_VAR_DECL,
  NT_ASSIGNMENT,
  NT_IDENTIFIER,
  NT_FN_DECL,
  NT_GIVE,
  NT_FN_CALL,
} NodeType;

typedef struct {
  struct s_ASTNode *left;
  struct s_ASTNode *right;
  char op;
} NodeOperation;

typedef struct {
  char name[64];
  char typeName[32];
  struct s_ASTNode *value;
} VarDecl;

typedef struct {
  char name[64];
  struct s_ASTNode *value;
} Assignment;

typedef struct {
  char name[64];
  char typeName[32];
} Parameter;

typedef struct {
  char name[64];
  char giveType[32];
  Parameter params[16];
  size_t paramCount;
  struct s_ASTNode *body[128];
  size_t statementCount;
} FnDecl;

typedef struct {
  struct s_ASTNode *value;
} GiveStatement;

typedef struct{
  char name[64];
  struct s_ASTNode *args[16];
  size_t argCount;
} FnCall;

typedef struct s_ASTNode {
  NodeType type;
  union {
    int number;
    char string[128];
    char identifier[64];
    NodeOperation nodeOperation;
    VarDecl varDecl;
    Assignment assignment;
    FnDecl fnDecl;
    GiveStatement giveStatement;
    FnCall fnCall;
  } NodeData;
} ASTNode;

ASTNode *CreateNumberNode(int number);

ASTNode *CreateStringNode(char *string);

ASTNode *CreateIdentifierNode(char *identifier);

ASTNode *CreateAssignmentNode(char *assignment, ASTNode *val);

ASTNode *CreateBinaryNode(char op, ASTNode *left, ASTNode *right);

ASTNode *CreateVarDeclNode(char *name, char *typeName, ASTNode *value);

ASTNode *CreateFnDeclNode(char *name, char *giveType, Parameter *params, size_t paramCount, ASTNode **body, size_t stmtCount);

ASTNode *CreateGiveNode(ASTNode *giveNode);

ASTNode *CreateFnCallNode(char *name, ASTNode **args, size_t argCount);

void FreeASTNode(ASTNode *node);

#endif
