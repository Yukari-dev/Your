#include "../include/lexer.h"
#include "../include/helper.h"
#include <ctype.h>
#include <stdlib.h>

Lexer *InitLexer(const char *source){
  Lexer *lexer = malloc(sizeof(Lexer));
  lexer->source = source;
  lexer->position = 0;
  lexer->line = 1;
  lexer->column = 1;
  return lexer;
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
      if (lexer->source[lexer->position] == '\\'){
        lexer->position++;
        lexer->column++;
        if(lexer->source[lexer->position]){
          if(lexer->source[lexer->position] == '\n'){
            lexer->line++;
            lexer->column = 1;
          } else
            lexer->column++;
          lexer->position++;
        }
        continue;
      }

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
    case '+':
      return MakeToken(TT_ADDITION, &lexer->source[start_pos], 1, lexer->line, start_col);
    case '-':
      return MakeToken(TT_SUBTRACT, &lexer->source[start_pos], 1, lexer->line, start_col);
    case '*':
      return MakeToken(TT_ASTERISK, &lexer->source[start_pos], 1, lexer->line, start_col);
    case '/':
      return MakeToken(TT_SLASH, &lexer->source[start_pos], 1, lexer->line, start_col);
    case '=':
      if (MatchChar(lexer, '='))
        return MakeToken(TT_EQUAL_EQUAL, &lexer->source[start_pos], 2, lexer->line, start_col);
      return MakeToken(TT_EQUAL, &lexer->source[start_pos], 1, lexer->line, start_col);
    case '>':
      if (MatchChar(lexer, '='))
        return MakeToken(TT_GREATER_OR_EQUAL, &lexer->source[start_pos], 2, lexer->line, start_col);
      return MakeToken(TT_GREATER, &lexer->source[start_pos], 1, lexer->line, start_col);
    case '<':
      if(MatchChar(lexer, '~'))
        return MakeToken(TT_CLOSE_SCOPE, &lexer->source[start_pos], 2, lexer->line, start_col);
      return MakeToken(TT_LESS, &lexer->source[start_pos], 1, lexer->line, start_col);
    case '~':
      if (MatchChar(lexer, '>'))
        return MakeToken(TT_OPEN_SCOPE, &lexer->source[start_pos], 2, lexer->line, start_col);
      while (lexer->source[lexer->position] != '\n' && lexer->source[lexer->position]){
        lexer->position++;
        lexer->column++;
      }
      return GetNextToken(lexer);
    case ':':
      return MakeToken(TT_COLON, &lexer->source[start_pos], 1, lexer->line, start_col);
    case ',':
      return MakeToken(TT_COMMA, &lexer->source[start_pos], 1, lexer->line, start_col);
    case '(':
      return MakeToken(TT_OPEN_PAREN, &lexer->source[start_pos], 1, lexer->line, start_col);
    case ')':
      return MakeToken(TT_CLOSE_PAREN, &lexer->source[start_pos], 1, lexer->line, start_col);
  }

  return MakeToken(TT_UNKNOWN, &lexer->source[start_pos], 1, lexer->line, start_col);
}

void FreeLexer(Lexer *lexer){
  free(lexer);
  lexer = NULL;
}
