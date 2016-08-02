#ifndef scene_hpp
#define scene_hpp

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <OpenGL/gl3.h>
#include "linear_math.hpp"
#include "shader.hpp"

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

struct directionalLight {
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

struct materialShader {
  GLuint ID;

  GLint viewPosLoc;
  GLint modelLoc;
  GLint normalMatrixLoc;
  GLint viewLoc;
  GLint projectionLoc;

  GLint worldAmbient;
  GLint matDiffuseLoc;
  GLint matSpecularLoc;
  GLint matShininessLoc;

  GLint *dirLightDirectionLocs;
  GLint *dirLightDiffuseLocs;

  GLint *pointLightLocationLocs;
  GLint *pointLightDiffuseLocs;
  GLint *pointLightConstantLocs;
  GLint *pointLightLinearLocs;
  GLint *pointLightQuadraticLocs;
};

struct scene {
  GLuint numMeshes;
  GLuint numEntities;
  GLuint numDirLights;
  GLuint numPointLights;
  GLuint VAO;
  GLuint VBO;
  GLuint EBO;
  materialShader matShader;
  vec3 skyColor;
  vec3 ambientColor;

  mesh *meshes;
  entity *entities;
  directionalLight *dirLights;
  pointLight *pointLights;
};

scene loadScene(const char *sceneName);
void deleteScene(scene s);
void drawScene(scene s);

#endif
