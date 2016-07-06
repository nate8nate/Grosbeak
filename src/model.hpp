#ifndef model_hpp
#define model_hpp

#include <stdio.h>
#include <stdlib.h>
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

struct model {
  mesh mesh;
  vec3 location;
  quat rotation;
  vec3 scale;
};

model loadModel(const char * path);
void destroyModel(model d);

#endif
