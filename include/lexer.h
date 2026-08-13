#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>

typedef enum {
  TT_VAR,
  TT_IDENTIFIER,
  TT_EOF,
  TT_UNKNOWN,
} TokenType;

typedef struct {
  TokenType tokenType;
  const char *buffer;
  size_t length;
  int line;
} Token;

char *read_file(FILE *file);

Token GetNextToken(const char *buffer, size_t *position);

void PrintTokens(const char *buffer);

Token Tokenize(FILE *file);

#endif
