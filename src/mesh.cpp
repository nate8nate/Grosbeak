#include "mesh.hpp"

mesh loadMesh(const char * path)
{
  FILE * meshFile = fopen(path, "r");
  mesh m;

  fscanf(meshFile, "%u", &m.numVertices);
  m.vertices = (struct vertex *)malloc(m.numVertices * sizeof(vertex));
  for (unsigned int i=0; i<m.numVertices; ++i)
  {
    fscanf(meshFile, "%f", &m.vertices[i].position.x);
    fscanf(meshFile, "%f", &m.vertices[i].position.y);
    fscanf(meshFile, "%f", &m.vertices[i].position.z);
    fscanf(meshFile, "%f", &m.vertices[i].normal.x);
    fscanf(meshFile, "%f", &m.vertices[i].normal.y);
    fscanf(meshFile, "%f", &m.vertices[i].normal.z);
  }

  fscanf(meshFile, "%u", &m.numIndices);
  m.indices = (unsigned short *)malloc(m.numIndices * sizeof(unsigned short));
  for (unsigned int i=0; i<m.numIndices; ++i)
  {
    fscanf(meshFile, "%hu", &m.indices[i]);
  }

  return m;
}

void destroyMesh(mesh m) {
  free(m.vertices);
  free(m.indices);
}