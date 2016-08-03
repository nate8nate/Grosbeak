#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#ifdef _WIN32

// WINDOWS

#else

#include "sys/stat.h"

#endif

bool fileExists(const char *path);

#endif
