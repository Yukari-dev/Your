#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include <stdbool.h>
#include <stddef.h>

typedef enum { 
  VT_BOOL, 
  VT_CHAR, 
  VT_STRING, 
  VT_NUMBER, 
  VT_NULL
} ValueType;

typedef struct {
  ValueType valueType;
  union {
    bool as_boolean;
    char as_char;
    char *as_string;
    double as_number;
  } as;
} Value;

typedef struct {
  char name[64];
  Value value;
} Symbol;

typedef struct{
  Symbol **symbols;
  size_t capacity;
  int count;
} SymbolTable;

SymbolTable *InitSymbolTable(int initialCapacity);

void SetSymbol(SymbolTable *table, char *name, Value val);
bool GetSymbol(SymbolTable *table, char *name, Value *out_val);

void PrintSymbolTable(SymbolTable *table);

Value MakeBoolean(bool b);
Value MakeNumber(double num);
Value MakeChar(char c);
Value MakeString(char *s);
Value MakeNull();

#endif
