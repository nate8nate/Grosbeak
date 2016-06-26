#include "io.hpp"

const char * fileToString(const char * filepath)
{
  FILE * f;
  unsigned long size;
  char * buffer;

  f = fopen(filepath, "r");
  if (!f) {
    fprintf(stderr, "Could not open %s\n", filepath);
  }

  fseek(f, 0, SEEK_END);
  size = (unsigned long)ftell(f);
  rewind(f);

  buffer = (char *)calloc(1, size+1);
  if (!buffer) {
    fclose(f);
    fprintf(stderr, "Memory allocation for %s failed\n", filepath);
    return "";
  }

  if (fread((void *) buffer, size, 1, f) != 1) {
    fclose(f);
    free((void *) buffer);
    fprintf(stderr, "Could not read %s\n", filepath);
    return "";
  }

  fclose(f);
  return buffer;
}
