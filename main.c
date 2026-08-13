#include "include/lexer.h"
#include <stdio.h>

int main(int ac, char *av[]) {
  FILE *file = fopen(av[1], "r");
  if (file == NULL) {
    perror("Error opening file");
    return -1;
  }
  char *buffer = read_file(file);
  PrintTokens(buffer);
  fclose(file);
  return 0;
}
