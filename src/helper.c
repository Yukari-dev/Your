#include "../include/helper.h"
#include <string.h>

long GetFileSize(FILE *file) {
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}

Token MakeToken(TokenType tokenType, const char *start, size_t length, size_t *position, size_t new_pos){
  *position = new_pos;
  return (Token){
    .tokenType = tokenType,
    .buffer = start,
    .length = length
  };
}

TokenType CheckKeyword(const char *buffer, size_t length){
  if(length == 3){
    if (strncmp(buffer, "int", length) == 0)
      return TT_INT;
    if (strncmp(buffer, "var", length) == 0)
      return TT_VAR;
  }
  if (length == 6 && strncmp(buffer, "string", length) == 0)
      return TT_STRING;
  return TT_IDENTIFIER;
}

size_t MatchSymbol(const char *buffer, size_t *position, char expected){
  size_t current = 1;
  size_t start = current;

  while (buffer[current] != expected){
    current++;
  }
  if (buffer[current] == expected){
    *position = current + 1;
    return current;
  }
  return -1;
}

void PrintTokens(const char *buffer){
  size_t position = 0;
  size_t token_count = 0;

  printf("\n============================ LEXER ============================\n");
  printf("%-5s | %-24s | %-6s | %s\n", "INDEX", "TOKEN TYPE", "LENGTH", "VALUE");
  printf("===============================================================\n");
  while (1){
    Token token = GetNextToken(buffer, &position);
    token_count++;

    printf("[%03zu] | %-24s | %-6zu | \"%.*s\"\n",
      token_count,
      TokenTypeToString(token.tokenType),
      token.length,
      (int)token.length,
      token.buffer
    );
    if(token.tokenType == TT_EOF)
      break;
  }
 
  printf("===============================================================\n");
  printf("Total Tokens: %zu\n\n", token_count);
}

const char *TokenTypeToString(TokenType type){
  switch (type) {
      case TT_IDENTIFIER:      return "TT_IDENTIFIER";
      case TT_VAR:             return "TT_VAR";
      case TT_NUMBER:          return "TT_NUMBER";
      case TT_INT:             return "TT_INT";
      case TT_STRING:          return "TT_STRING";
      case TT_STRING_LITERAL:  return "TT_STRING_LITERAL";
      case TT_EQUAL:           return "TT_EQUAL";
      case TT_VAR_TYPE_DEC:    return "TT_VAR_TYPE_DEC";
      case TT_EOF:             return "TT_EOF";
      case TT_UNKNOWN:         return "TT_UNKNOWN";
      default:                 return "TT_UNKNOWN";
  }
}
