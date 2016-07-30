#ifndef scene_hpp
#define scene_hpp

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <OpenGL/gl3.h>
#include "linear_math.hpp"

struct vertex {
  vec3 position;
  vec3 normal;
  vec3 color;
};

struct mesh {
  GLuint numVertices;
  GLuint numVertexIndices;

  vertex *vertices;
  GLushort *vertexIndices;
};

struct material {
  vec3 diffuse;
  vec3 specular;
  GLfloat shininess;
};

struct entity {
  GLuint meshIndex;
  vec3 location;
  quat rotation;
  vec3 scale;
  material material;
};

struct dirLight {
  vec3 direction;
  vec3 diffuse;
};

struct pointLight {
  vec3 location;
  GLfloat constant;
  GLfloat linear;
  GLfloat quadratic;
  vec3 diffuse;
};

struct scene {
  GLuint numVertices;
  GLuint numMeshes;
  GLuint numEntities;
  GLuint numDirLights;
  GLuint numPointLights;
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  vec3 ambientColor;

  // unsigned int shader;
  mesh *meshes;
  entity *entities;
  dirLight *dirLights;
  pointLight *pointLights;
};

scene loadScene(const char *sceneName);
void draw(scene s);

#endif
