#ifndef SHADER_H
#define SHADER_H

#include "io.hpp"
#include <OpenGL/gl3.h>
#include <stdlib.h>
#include <string.h>

GLuint loadShadersFV(const char *shaderName);
GLuint loadShadersFGV(const char *shaderName);

#endif
