#include "platform.hpp"

#ifdef _WIN32

#else

bool fileExists(const char *path) {
  struct stat buffer;
  bool exists = !stat(path, &buffer); //TODO test nullpntr
  return exists;
}

#endif
