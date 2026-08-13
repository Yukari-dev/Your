#include <stdio.h>

int main(int ac, char *av[]) {
  FILE *file = fopen(av[1], "r");
  if (file == NULL) {
    perror("Error opening file");
    return -1;
  }
  fseek(file, 0, SEEK_END);
  printf("%ld\n", ftell(file));
  fclose(file);
  return 0;
}
