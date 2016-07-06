#include "model.hpp"

model loadModel(const char * path) {
  FILE * modelFile = fopen(path, "r");
  model d;

  fscanf(modelFile, "%f", &d.location.x);
  fscanf(modelFile, "%f", &d.location.y);
  fscanf(modelFile, "%f", &d.location.z);
  fscanf(modelFile, "%f", &d.rotation.r);
  fscanf(modelFile, "%f", &d.rotation.i);
  fscanf(modelFile, "%f", &d.rotation.j);
  fscanf(modelFile, "%f", &d.rotation.k);
  fscanf(modelFile, "%f", &d.scale.x);
  fscanf(modelFile, "%f", &d.scale.y);
  fscanf(modelFile, "%f", &d.scale.z);

  fscanf(modelFile, "%u", &d.mesh.numVertices);
  d.mesh.vertices = (struct vertex *)malloc(d.mesh.numVertices * sizeof(vertex));
  for (unsigned int i=0; i<d.mesh.numVertices; ++i)
  {
    fscanf(modelFile, "%f", &d.mesh.vertices[i].position.x);
    fscanf(modelFile, "%f", &d.mesh.vertices[i].position.y);
    fscanf(modelFile, "%f", &d.mesh.vertices[i].position.z);
    fscanf(modelFile, "%f", &d.mesh.vertices[i].normal.x);
    fscanf(modelFile, "%f", &d.mesh.vertices[i].normal.y);
    fscanf(modelFile, "%f", &d.mesh.vertices[i].normal.z);
  }

  fscanf(modelFile, "%u", &d.mesh.numIndices);
  d.mesh.indices = (unsigned short *)malloc(d.mesh.numIndices * sizeof(unsigned short));
  for (unsigned int i=0; i<d.mesh.numIndices; ++i)
  {
    fscanf(modelFile, "%hu", &d.mesh.indices[i]);
  }

  return d;
}

void destroyModel(model d) {
  free(d.mesh.vertices);
  free(d.mesh.indices);
}
