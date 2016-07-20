#ifndef SHADER_H
#define SHADER_H

#include "io.hpp"
#include <OpenGL/gl3.h>
#include <stdlib.h>

GLuint loadShaders(const char *vertexPath, const char *fragmentPath);
GLuint loadShaders(const char *vertexPath, const char *geometryPath, const char *fragmentPath);

#endif
