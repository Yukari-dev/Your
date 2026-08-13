#include "../include/helper.h"
#include <stdlib.h>
#include <string.h>

long GetFileSize(FILE *file) {
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}

char *read_file(FILE *file) {
  long file_size = GetFileSize(file);
  char *buffer = malloc(sizeof(char) * (file_size + 1));
  fread(buffer, 1, file_size, file);
  buffer[file_size] = '\0';
  return buffer;
}

Token MakeToken(TokenType tokenType, const char *start, size_t length, int line,
                int column) {
  return (Token){.tokenType = tokenType,
                 .buffer = start,
                 .length = length,
                 .line = line,
                 .column = column};
}

TokenType CheckKeyword(const char *buffer, size_t length) {
  if (length == 3) {
    if (strncmp(buffer, "int", length) == 0)
      return TT_INT;
    if (strncmp(buffer, "var", length) == 0)
      return TT_VAR;
  }
  if (length == 6 && strncmp(buffer, "string", length) == 0)
    return TT_STRING;
  return TT_IDENTIFIER;
}

size_t MatchSymbol(const char *buffer, size_t *position, char expected) {
  size_t current = 1;
  size_t start = current;

  while (buffer[current] != expected) {
    current++;
  }
  if (buffer[current] == expected) {
    *position = current + 1;
    return current;
  }
  return -1;
}

bool MatchChar(Lexer *lexer, char expected) {
  if (lexer->source[lexer->position] == expected) {
    lexer->position++;
    lexer->column++;
    return true;
  }
  return false;
}

void PrintToken(Token token, size_t token_count, char *pos_buf) {
  printf("[%03zu] | %-8s | %-24s | %-6zu | \"%.*s\"\n", token_count, pos_buf,
         TokenTypeToString(token.tokenType), token.length, (int)token.length,
         token.buffer);
}

void PrintTokens(const char *buffer) {
  Lexer lexer = InitLexer(buffer);
  size_t token_count = 0;

  printf("\n============================ LEXER ============================\n");
  printf("%-5s | %-7s | %-24s | %-6s | %s\n", "INDEX", "LINE:COL", "TOKEN TYPE",
         "LENGTH", "VALUE");
  printf("===============================================================\n");
  while (1) {
    Token token = GetNextToken(&lexer);
    token_count++;

    char pos_buf[16];
    snprintf(pos_buf, sizeof(pos_buf), "%d:%d", token.line, token.column);
    PrintToken(token, token_count, pos_buf);
    if (token.tokenType == TT_EOF)
      break;
  }

  printf("===============================================================\n");
  printf("Total Tokens: %zu\n\n", token_count);
}

void PrintAST(ASTNode *node, int level) {
  if(!node)
    return;
  for(size_t i = 0; i < level * 2; i++) {
    printf(" ");
  }
  if(node->type == NT_NUMBER)
    printf("[NUMBER: %d]\n", node->NodeData.number);
  else{
    printf("[OP: %c]\n", node->NodeData.nodeOperation.op);
    PrintAST(node->NodeData.nodeOperation.left, level + 1);
    PrintAST(node->NodeData.nodeOperation.right, level + 1);
  }
}

const char *TokenTypeToString(TokenType type) {
  switch (type) {
  case TT_IDENTIFIER:
    return "TT_IDENTIFIER";
  case TT_VAR:
    return "TT_VAR";
  case TT_NUMBER:
    return "TT_NUMBER";
  case TT_INT:
    return "TT_INT";
  case TT_STRING:
    return "TT_STRING";
  case TT_STRING_LITERAL:
    return "TT_STRING_LITERAL";
  case TT_EQUAL:
    return "TT_EQUAL";
  case TT_EQUAL_EQUAL:
    return "TT_EQUAL_EQUAL";
  case TT_GREATER_OR_EQUAL:
    return "TT_GREATER_OR_EQUAL";
  case TT_GREATER:
    return "TT_GREATER";
  case TT_ADDITION:
    return "TT_ADDITION";
  case TT_SUBTRACT:
    return "TT_SUBTRACT";
  case TT_ASTERISK:
    return "TT_ASTERISK";
  case TT_SLASH:
    return "TT_SLASH";
  case TT_EOF:
    return "TT_EOF";
  case TT_UNKNOWN:
    return "TT_UNKNOWN";
  default:
    return "TT_UNKNOWN";
  }
}
