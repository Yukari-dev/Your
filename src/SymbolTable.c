#include "../include/SymbolTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SymbolTable *InitSymbolTable(){
  SymbolTable *symbolTable = malloc(sizeof(SymbolTable));
  if(!symbolTable) return NULL;

  symbolTable->count = 0;
  symbolTable->capacity = 16;
  symbolTable->symbols = malloc(symbolTable->capacity * sizeof(Symbol *));
  symbolTable->parent = NULL;

  return symbolTable;
}

SymbolTable *CreateScope(SymbolTable *parent){
  SymbolTable *symbolTable = InitSymbolTable();
  symbolTable->parent = parent;
  return symbolTable;
}

void FreeScope(SymbolTable *scope){
  if(!scope) return;

  for(size_t i = 0; i < scope->count; i++){
    free(scope->symbols[i]);
  }
  free(scope->symbols);
  free(scope);
}

void SetSymbol(SymbolTable *table, char *name, Value val){
  if(table->count >= table->capacity){
    table->capacity *= 2;
    table->symbols = realloc(table->symbols, table->capacity * sizeof(Symbol*));
  }
  for(size_t i = 0; i < table->count; i++){
    if (strcmp(table->symbols[i]->name, name) == 0){
      table->symbols[i]->value = val;
      return;
    }
  }
  table->symbols[table->count] = malloc(sizeof(Symbol));
  strncpy(table->symbols[table->count]->name, name, 63);
  table->symbols[table->count]->name[63] = '\0';
  table->symbols[table->count]->value = val;
  table->count++;
}

bool UpdateSymbol(SymbolTable *table, char *name, Value val) {
  for (SymbolTable *current = table; current != NULL; current = current->parent) {
    for (size_t i = 0; i < current->count; i++) {
      if (strcmp(current->symbols[i]->name, name) == 0) {
        current->symbols[i]->value = val;
        return true;
      }
    }
  }

  return false;
}

bool GetSymbol(SymbolTable *table, char *name, Value *outVal){
  for(SymbolTable *current = table; current != NULL; current = current->parent){
    for(size_t i = 0; i < current->count; i++){
      if (strcmp(current->symbols[i]->name, name) == 0){
        *outVal = current->symbols[i]->value;
        return true;
      }
    }
  }
  if (table->parent != NULL) {
    return GetSymbol(table->parent, name, outVal);
  }
  return false;
}

void PrintSymbolTable(SymbolTable *table) {
  printf("\n=== SYMBOL TABLE(%d variables) ===\n", table->count);
  
  if (table->count == 0) {
      printf("  (empty)\n");
  }

  for (size_t i = 0; i < table->count; i++) {
    Symbol *sym = table->symbols[i];
    
    printf("  [%zu] %-10s = ", i + 1, sym->name);

    if (sym->value.valueType == VT_NUMBER) {
      printf("%g (number)\n", sym->value.as.as_number);
    } else if (sym->value.valueType == VT_STRING) {
      printf("\"%s\" (string)\n", sym->value.as.as_string);
    } else {
      printf("null\n");
    }
  }
  printf("===========================================\n\n");
}

void FreeSymbolTable(SymbolTable *table){
  if(!table) return;
  for(size_t i = 0; i < table->count; i++)
    free(table->symbols[i]);
  free(table->symbols);
  free(table);
}

Value MakeBoolean(bool b) {
  return (Value){.valueType = VT_BOOL, .as.as_boolean = b};
}

Value MakeNumber(double num) {
  return (Value){.valueType = VT_NUMBER, .as.as_number = num};
}

Value MakeChar(char c) {
  return (Value){.valueType = VT_CHAR, .as.as_char = c};
}

Value MakeString(char *s) {
  return (Value){.valueType = VT_STRING, .as.as_string = s};
}

Value MakeNull() {
  return (Value){.valueType = VT_NULL};
}
