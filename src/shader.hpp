#ifndef SHADER_H
#define SHADER_H

#include <stdlib.h>
#include <string.h>
#include <OpenGL/gl3.h>
#include "platform.hpp"
#include "io.hpp"

GLuint loadShaders(const char *shaderName, unsigned int numDirLights, unsigned int numPointLights);

#endif
