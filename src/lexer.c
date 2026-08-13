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
  int line = 1;
  int column = 0;
  while (buffer[current] && isspace(buffer[current])){
    if(buffer[current] == '\n')
      line++;
    column++;
    current++;
  }

  if (buffer[current] == '\0'){
    *position = current;
    return MakeToken(TT_EOF, &buffer[current], 0, position, current);
  }
  
  if(isalpha(buffer[current]) || buffer[current] == '_'){
    size_t start = current;
    while (isalnum(buffer[current]) || buffer[current] == '_')
      current++;
    size_t length = current - start;
    TokenType type = CheckKeyword(&buffer[start], length);
    return MakeToken(type, &buffer[start], length, position, current);
  }
  if(isdigit(buffer[current])){
    size_t start = current;
    while (isdigit(buffer[current]))
      current++;
    return MakeToken(TT_NUMBER, &buffer[start], current - start, position, current);
  }
  size_t start = current;
  switch(buffer[current]){
    case '=': return MakeToken(TT_EQUAL, &buffer[start], 1, position, current + 1);
    case '>': return MakeToken(TT_VAR_TYPE_DEC, &buffer[start], 1, position, current + 1);
    case '"':
      size_t start = current;
      current++;
      while (buffer[current] != '"' && buffer[current] != '\0')
        current++;
      if (buffer[current] == '\0')
        return MakeToken(TT_UNKNOWN, &buffer[start], 1, position, current + 1);
      current++;
      size_t content_length = current - start - 2;
      return MakeToken(TT_STRING_LITERAL, &buffer[start + 1], content_length, position, current);
  }

  return MakeToken(TT_UNKNOWN, &buffer[start], 1, position, current + 1);
}

Token Tokenize(FILE *file) {
  char *buffer = read_file(file);

  return (Token){0};
}
