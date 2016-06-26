#ifndef mesh_hpp
#define mesh_hpp

#include <stdio.h>
#include <stdlib.h>
#include "io.hpp"
#include "linear_math.hpp"

struct vertex {
  vec3 position;
  vec3 normal;
};

struct mesh {
  unsigned int numVertices;
  unsigned int numIndices;

  vertex * vertices;
  unsigned short * indices;
};

mesh loadMesh(const char * path);
void destroyMesh(mesh m);

#endif
