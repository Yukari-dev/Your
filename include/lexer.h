#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>

typedef enum {
  TT_VAR,
  TT_IDENTIFIER,
  TT_EQUAL,
  TT_VAR_TYPE_DEC,
  TT_INT,
  TT_STRING,
  TT_STRING_LITERAL,
  TT_NUMBER,
  TT_EOF,
  TT_UNKNOWN,
} TokenType;

typedef struct {
  TokenType tokenType;
  const char *buffer;
  size_t length;
  int line;
  int column;
} Token;

typedef struct{
  const char *source;
  size_t position;
  int line;
  int column;
} Lexer;

Lexer InitLexer(const char *source);

char *read_file(FILE *file);

Token GetNextToken(Lexer *lexer);

Token Tokenize(FILE *file);

#endif
