#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>

typedef enum {
  TT_UNKNOWN = 0,
  TT_VAR,
  TT_IDENTIFIER,
  TT_EQUAL,
  TT_EQUAL_EQUAL,
  TT_GREATER,
  TT_GREATER_OR_EQUAL,
  TT_LESS,
  TT_LESS_OR_EQUAL,
  TT_COLON,
  TT_ADDITION,
  TT_SUBTRACT,
  TT_ASTERISK,
  TT_SLASH,
  TT_VAR_TYPE,
  TT_INT,
  TT_STRING,
  TT_STRING_LITERAL,
  TT_NUMBER,
  TT_TILDE,
  TT_FN_DECL,
  TT_GIVE,
  TT_COMMA,
  TT_OPEN_SCOPE,
  TT_CLOSE_SCOPE,
  TT_OPEN_PAREN,
  TT_CLOSE_PAREN,
  TT_WHEN,
  TT_OTHERWISE,
  TT_EOF,
} TokenType;

typedef struct {
  TokenType tokenType;
  const char *buffer;
  size_t length;
  int line;
  int column;
} Token;

typedef struct {
  const char *source;
  size_t position;
  int line;
  int column;
} Lexer;

Lexer *InitLexer(const char *source);

char *read_file(FILE *file);

Token GetNextToken(Lexer *lexer);

void FreeLexer(Lexer *lexer);

#endif
