#include "io.h"

char* readFile(const char *filepath)
{
  FILE *f;
  long size;
  char *buffer;

  f = fopen(filepath, "r");
  if (!f) {
    fprintf(stderr, "Could not open %s\n", filepath);
  }

  fseek(f, 0, SEEK_END);
  size = ftell(f);
  rewind(f);

  buffer = malloc(size+1);
  if (!buffer) {
    fclose(f);
    fprintf(stderr, "Memory allocation reading %s failed\n", filepath);
    return "";
  }

  if (fread(buffer, size, 1, f) != 1) {
    fclose(f);
    free(buffer);
    fprintf(stderr, "Could not read %s\n", filepath);
    return "";
  }

  fclose(f);
  buffer[size] = '\0';
  return buffer;
}
