#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>

typedef enum {
  TT_VAR,
  TT_IDENTIFIER,
  TT_ID,
  TT_EOF,
} TokenType;

typedef struct {
  TokenType tokenType;
  char *value;
} Token;

char *read_file(FILE *file);

#endif
