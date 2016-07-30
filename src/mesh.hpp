#ifndef mesh_hpp
#define mesh_hpp

#include <stdio.h>
#include <stdlib.h>
#include <OpenGL/gl3.h>
#include "linear_math.hpp"



void draw(mesh m, mat4 model);
mesh loadMesh(const char *path, scene *s);
void destroyMesh(mesh m);


#endif
