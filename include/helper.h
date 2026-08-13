#ifndef HELPER_H
#define HELPER_H
#include <stdio.h>
#include "lexer.h"

long GetFileSize(FILE *file);

Token MakeToken(TokenType tokenType, const char *start, size_t length, size_t *position, size_t new_pos);

TokenType CheckKeyword(const char *buffer, size_t length);

const char *TokenTypeToString(TokenType type);

size_t MatchSymbol(const char *buffer, size_t *position, char expected);

void PrintTokens(const char *buffer);

#endif
