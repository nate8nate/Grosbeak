#ifndef gouraud_hpp
#define gouraud_hpp

#include <OpenGL/gl3.h>
#include "shader.hpp"

GLuint generateGouraudShader(unsigned int numDirLights, unsigned int numPointLights);

#endif