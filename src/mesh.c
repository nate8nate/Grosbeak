#include "mesh.h"

void loadMesh(struct mesh m,
              const char * path)
{
  FILE * meshFile = fopen(path, "r");

  fscanf(meshFile, "%ud", &m.numVertices);
  m.vertices = malloc(m.numVertices * sizeof(float));
  for (unsigned int i=0; i<m.numVertices; i++)
  {
    fscanf(meshFile, "%f", &m.vertices[i]);
  }

  fscanf(meshFile, "%ud", &m.numFaces);
  m.faces = malloc(m.numFaces * sizeof(unsigned int));
  for (unsigned int i=0; i<m.numFaces; i++)
  {
    fscanf(meshFile, "%u", &m.faces[i]);
  }

  fscanf(meshFile, "%ud", &m.numNormals);
  m.normals = malloc(m.numNormals * sizeof(float));
  for (unsigned int i=0; i<m.numNormals; i++)
  {
    fscanf(meshFile, "%f", &m.normals[i]);
  }
}
