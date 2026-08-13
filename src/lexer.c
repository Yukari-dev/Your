#include "../include/lexer.h"
#include "../include/helper.h"
#include <ctype.h>
#include <stdlib.h>

Lexer InitLexer(const char *source){
  return (Lexer){
    .source = source,
    .position = 0,
    .line = 1,
    .column = 1
  };
}

Token GetNextToken(Lexer *lexer) {
  while (lexer->source[lexer->position] && isspace(lexer->source[lexer->position])){
    if (lexer->source[lexer->position] == '\n'){
      lexer->line++;
      lexer->column = 1;
    } else
      lexer->column++;
    lexer->position++;
  }

  if(lexer->source[lexer->position] == '\0')
    return MakeToken(TT_EOF, &lexer->source[lexer->position], 0, lexer->line, lexer->column);

  size_t start_pos = lexer->position;
  int start_col = lexer->column;
  if(isalpha(lexer->source[lexer->position]) || lexer->source[lexer->position] == '_'){
    while(isalnum(lexer->source[lexer->position]) || lexer->source[lexer->position] == '_'){
      lexer->position++;
      lexer->column++;
    }

    int length = lexer->position - start_pos;
    TokenType type = CheckKeyword(&lexer->source[start_pos], length);
    return MakeToken(type, &lexer->source[start_pos], length, lexer->line, start_col);
  }

  if(isdigit(lexer->source[lexer->position])){
    while(isdigit(lexer->source[lexer->position])){
      lexer->position++;
      lexer->column++;
    }
    return MakeToken(TT_NUMBER, &lexer->source[start_pos], lexer->position - start_pos, lexer->line, start_col);
  }
  
  char current_char = lexer->source[lexer->position];
  if(current_char == '"'){
    lexer->position++;
    lexer->column++;
    while(lexer->source[lexer->position] != '"' && lexer->source[lexer->position]){
      if(lexer->source[lexer->position] == '\n'){
        lexer->line++;
        lexer->column = 1;
      } else
        lexer->column++;
      lexer->position++;
    }
    if(lexer->source[lexer->position] == '\0')
      return MakeToken(TT_UNKNOWN, &lexer->source[start_pos], lexer->position - start_pos, lexer->line, start_col);
    lexer->position++;
    lexer->column++;
    int content_length = lexer->position - start_pos - 2;
    return MakeToken(TT_STRING_LITERAL, &lexer->source[start_pos + 1], content_length, lexer->line, start_col);
  }
  lexer->position++;
  lexer->column++;

  switch(current_char){
    case '=':
      return MakeToken(TT_EQUAL, &lexer->source[start_pos], 1, lexer->line, start_col);
    case '>':
      return MakeToken(TT_VAR_TYPE_DEC, &lexer->source[start_pos], 1, lexer->line, start_col);
  }

  return MakeToken(TT_UNKNOWN, &lexer->source[start_pos], 1, lexer->line, lexer->column);
}

Token Tokenize(FILE *file) {
  char *buffer = read_file(file);

  return (Token){0};
}
