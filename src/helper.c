#include "../include/helper.h"
#include <ctype.h>

long GetFileSize(FILE *file) {
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}

char *GetWord(const char *buffer, long position) {
  size_t current = position;
  while (isspace(buffer[current]))
    current++;

  if (isalpha(buffer[current]) || buffer[current] == '_'){
    size_t start = current;

    while (isalnum(buffer[current]) || buffer[current] == '_')
      current++;
    size_t length = current - start;
  }
}
