#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "sys/stat.h"

#ifdef _WIN32

// WINDOWS

#else

bool fileExists(const char *path) {
  struct stat *buffer;
  return stat(path, buffer); //TODO test nullpntr
}

#endif

#endif
