#ifndef HELPER_H
#define HELPER_H
#include <stdio.h>
#include <stdbool.h>
#include "lexer.h"

long GetFileSize(FILE *file);

char *read_file(FILE *file);

Token MakeToken(TokenType tokenType, const char *start, size_t length, int line, int column);

TokenType CheckKeyword(const char *buffer, size_t length);

const char *TokenTypeToString(TokenType type);

size_t MatchSymbol(const char *buffer, size_t *position, char expected);

bool MatchChar(Lexer *lexer, char expected);

void PrintTokens(const char *buffer);

#endif
