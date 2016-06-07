#ifndef mesh_h
#define mesh_h

#include <stdio.h>
#include <stdlib.h>
#include "io.h"

struct mesh {
  unsigned int numVertices;
  unsigned int numFaces;
  unsigned int numNormals;

  float * vertices;
  unsigned int * faces;
  float * normals;
};

void loadMesh(struct mesh m, const char * path);

#endif
