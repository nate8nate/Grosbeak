#include "mesh.hpp"

void draw(mesh m, mat4 model) {
  GLuint modelLoc;
  GLuint normalMatrixLoc;

  mat4 normalMatrix = transpose(inverse(model));

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.flat);
  glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix.flat);
}

mesh loadMesh(const char * path, scene *s) {
  FILE *meshFile = fopen(path, "r");
  mesh m;

  fscanf(meshFile, "%u", &m.numVertices);
  m.vertices = (vertex *)malloc(m.numVertices * sizeof(vertex));
  for (unsigned int i=0; i<m.numVertices; ++i)
  {
    fscanf(meshFile, "%f,%f,%f",
           &m.vertices[i].position.x,
           &m.vertices[i].position.y,
           &m.vertices[i].position.z);
    fscanf(meshFile, "%f,%f,%f",
           &m.vertices[i].normal.x,
           &m.vertices[i].normal.y,
           &m.vertices[i].normal.z);
    fscanf(meshFile, "%f,%f,%f",
           &m.vertices[i].color.x,
           &m.vertices[i].color.y,
           &m.vertices[i].color.z);
  }

  fscanf(meshFile, "%u", &m.numIndices);
  m.indices = (unsigned short *)malloc(m.numIndices * sizeof(unsigned short));
  for (unsigned int i=0; i<m.numIndices; ++i)
  {
    fscanf(meshFile, "%hu", &m.indices[i]);
  }

  return m;
}

void destroyModel(mesh m) {
  free(m.vertices);
  free(m.indices);
}
