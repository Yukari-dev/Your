#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdbool.h>
#include <stddef.h>
#include "AST.h"

typedef enum { 
  VT_BOOL, 
  VT_CHAR, 
  VT_STRING, 
  VT_NUMBER, 
  VT_FN,
  VT_NULL
} ValueType;

typedef struct {
  ValueType valueType;
  union {
    bool as_boolean;
    char as_char;
    char *as_string;
    double as_number;
    struct s_ASTNode *as_ast;
  } as;
} Value;

typedef struct {
  char name[64];
  Value value;
} Symbol;

typedef struct SymbolTable{
  Symbol **symbols;
  size_t capacity;
  int count;
  struct SymbolTable *parent;
} SymbolTable;

SymbolTable *InitSymbolTable();

SymbolTable *CreateScope(SymbolTable *parent);
void FreeScope(SymbolTable *scope);

void SetSymbol(SymbolTable *table, char *name, Value val);
bool GetSymbol(SymbolTable *table, char *name, Value *out_val);

void PrintSymbolTable(SymbolTable *table);

void FreeSymbolTable(SymbolTable *table);

Value MakeBoolean(bool b);
Value MakeNumber(double num);
Value MakeChar(char c);
Value MakeString(char *s);
Value MakeNull();

#endif
