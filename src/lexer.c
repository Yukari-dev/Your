#include "../include/lexer.h"
#include "../include/helper.h"
#include <ctype.h>
#include <stdlib.h>

char *read_file(FILE *file) {
  long file_size = GetFileSize(file);
  char *buffer = malloc(sizeof(char) * (file_size + 1));
  fread(buffer, 1, file_size, file);
  buffer[file_size] = '\0';
  return buffer;
}

Token GetNextToken(const char *buffer, size_t *position) {
  size_t current = *position;
  while (buffer[current] != '\0' && isspace(buffer[current]))
    current++;

  if (buffer[current] == '\0') {
    *position = current;
    return (Token){
        .tokenType = TT_EOF, .buffer = &buffer[current], .length = 0};
  }

  if (isalpha(buffer[current]) || buffer[current] == '_') {
    size_t start = current;

    while (isalnum(buffer[current]) || buffer[current] == '_')
      current++;

    size_t length = current - start;
    *position = current;
    return (Token){
        .tokenType = TT_IDENTIFIER,
        .buffer = &buffer[start],
        .length = length,
    };
  }
  size_t start = current;
  current++;
  *position = current;

  return (Token){
    .tokenType = TT_UNKNOWN, 
    .buffer = &buffer[start], 
    .length = 1
  };
}

void PrintTokens(const char *buffer) {
  size_t position = 0;
  while (buffer[position]) {
    Token token = GetNextToken(buffer, &position);
    if (token.tokenType == TT_EOF)
      break;
    for(size_t i = 0; i < token.length; i++){
      printf("%c", token.buffer[i]);
    }
    printf(" ");
    switch(token.tokenType){
      case TT_VAR:
        printf("VAR\n");
      case TT_IDENTIFIER:
        printf("IDENTIFIER\n");
      case TT_UNKNOWN:
        printf("UNKNOWN\n");
      case TT_EOF:
        printf("EOF\n");
    }
  }
}

Token Tokenize(FILE *file) {
  char *buffer = read_file(file);

  return (Token){0};
}
