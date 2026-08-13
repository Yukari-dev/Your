#include "include/helper.h"
#include "include/lexer.h"
#include "include/parser.h"
#include <stdio.h>

int main(int ac, char *av[]) {
  FILE *file = fopen(av[1], "r");
  if (file == NULL) {
    perror("Error opening file");
    return -1;
  }
  char *source = read_file(file);
  Lexer lexer = InitLexer(source);
  Parser *parser = InitParser(&lexer);
  AdvanceParser(parser);
  AdvanceParser(parser);
  AdvanceParser(parser);
  AdvanceParser(parser);
  AdvanceParser(parser);
  ASTNode *node = ParseExpression(parser);
  PrintAST(node, 0);
  // PrintTokens(source);
  fclose(file);
  return 0;
}
