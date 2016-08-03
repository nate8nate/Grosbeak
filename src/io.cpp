#include "io.hpp"

const char *fileToChars(const char *filepath)
{
  static char *result = NULL;

  FILE *file;
  file = fopen(filepath, "r");
  if (file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    result = (char *)malloc(size+1);
    fread(result, size, 1, file);
    result[size] = '\0';

    fclose(file);
  }

  return result;
}
