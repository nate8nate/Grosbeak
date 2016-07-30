#include "scene.hpp"

const char sceneDir[] = "scenes/";
const char meshDir[] = "meshes/";

const char sceneExt[] = ".scene";
const char meshExt[] = ".mesh";

void drawScene(scene s) {
  for (unsigned int i=0; i<s.numEntities; ++i) {
    // draw(s.entities[i]);
  }
}

scene loadScene(const char *sceneName) {
  char scenePath[128];
  strcpy(scenePath, sceneDir);
  strcat(scenePath, sceneName);
  strcat(scenePath, sceneExt);
  FILE * sceneFile = fopen(scenePath, "r");
  scene s;

  unsigned int numSceneVertices = 0;
  unsigned int numSceneVertexIndices = 0;

  fscanf(sceneFile, "%u", &s.numMeshes);
  s.meshes = (mesh *)malloc(s.numMeshes * sizeof(mesh));
  for (unsigned int m=0; m<s.numMeshes; m++) {
    char meshName[64];
    fscanf(sceneFile, " %64s", meshName);

    char meshPath[128];
    strcpy(meshPath, meshDir);
    strcat(meshPath, meshName);
    strcat(meshPath, meshExt);
    FILE *meshFile = fopen(meshPath, "r");

    fscanf(meshFile, "%u", &s.meshes[m].numVertices);
    s.meshes[m].vertices = (vertex *)malloc(s.meshes[m].numVertices * sizeof(vertex));
    for (unsigned int v=0; v<s.meshes[m].numVertices; v++) {
      fscanf(meshFile, "%f,%f,%f\t%f,%f,%f\t%f,%f,%f",
                        &s.meshes[m].vertices[v].position.x,
                        &s.meshes[m].vertices[v].position.y,
                        &s.meshes[m].vertices[v].position.z,
                        &s.meshes[m].vertices[v].normal.x,
                        &s.meshes[m].vertices[v].normal.y,
                        &s.meshes[m].vertices[v].normal.z,
                        &s.meshes[m].vertices[v].color.r,
                        &s.meshes[m].vertices[v].color.g,
                        &s.meshes[m].vertices[v].color.b);
    }
    fscanf(meshFile, "%u", &s.meshes[m].numVertexIndices);
    s.meshes[m].vertexIndices = (unsigned short *)malloc(s.meshes[m].numVertexIndices * sizeof(short));
    for (unsigned int i=0; i<s.meshes[m].numVertexIndices; i=i+3) {
      fscanf(meshFile, "%hu\t%hu\t%hu",
                        &s.meshes[m].vertexIndices[i],
                        &s.meshes[m].vertexIndices[i+1],
                        &s.meshes[m].vertexIndices[i+2]);
    }

    numSceneVertices += s.meshes[m].numVertices;
    numSceneVertexIndices += s.meshes[m].numVertexIndices;

    fclose(meshFile);
  }

  fscanf(sceneFile, "%u", &s.numEntities);
  s.entities = (entity *)malloc(s.numEntities * sizeof(entity));
  for (unsigned int e=0; e<s.numEntities; e++)
  {
    fscanf(sceneFile, "%u", &s.entities[e].meshIndex);
    fscanf(sceneFile, "%f,%f,%f",
                        &s.entities[e].location.x,
                        &s.entities[e].location.y,
                        &s.entities[e].location.z);
    fscanf(sceneFile, "%f,%f,%f,%f",
                        &s.entities[e].rotation.r,
                        &s.entities[e].rotation.i,
                        &s.entities[e].rotation.j,
                        &s.entities[e].rotation.k);
    fscanf(sceneFile, "%f,%f,%f",
                        &s.entities[e].scale.x,
                        &s.entities[e].scale.y,
                        &s.entities[e].scale.z);
  }

  fscanf(sceneFile, "%u", &s.numDirLights);
  s.dirLights = (dirLight *)malloc(s.numDirLights * sizeof(dirLight));
  for (unsigned int d=0; d<s.numDirLights; d++)
  {
    fscanf(sceneFile, "%f,%f,%f",
                          &s.dirLights[d].direction.x,
                          &s.dirLights[d].direction.y,
                          &s.dirLights[d].direction.z);
    fscanf(sceneFile, "%f,%f,%f",
                          &s.dirLights[d].diffuse.r,
                          &s.dirLights[d].diffuse.g,
                          &s.dirLights[d].diffuse.b);
  }

  fscanf(sceneFile, "%u", &s.numPointLights);
  s.pointLights = (pointLight *)malloc(s.numPointLights * sizeof(pointLight));
  for (unsigned int p=0; p<s.numPointLights; p++)
  {
    fscanf(sceneFile, "%f,%f,%f",
                            &s.pointLights[p].location.x,
                            &s.pointLights[p].location.y,
                            &s.pointLights[p].location.z);
    fscanf(sceneFile, "%f", &s.pointLights[p].constant);
    fscanf(sceneFile, "%f", &s.pointLights[p].linear);
    fscanf(sceneFile, "%f", &s.pointLights[p].quadratic);
    fscanf(sceneFile, "%f,%f,%f",
                            &s.pointLights[p].diffuse.r,
                            &s.pointLights[p].diffuse.g,
                            &s.pointLights[p].diffuse.b);
  }

  fscanf(sceneFile, "%f,%f,%f",
                    &s.ambientColor.r,
                    &s.ambientColor.g,
                    &s.ambientColor.b);

  fclose(sceneFile);

  vertex *sceneVertices = (vertex *)malloc(numSceneVertices * sizeof(vertex));
  unsigned short *sceneVertexIndices = (unsigned short *)malloc(numSceneVertexIndices * sizeof(short));
  unsigned int vertexOffset = 0;
  unsigned int vertexIndexOffset = 0;
  for (unsigned int m=0; m<s.numMeshes; m++) {
    for (unsigned int v=0; v<s.meshes[m].numVertices; v++) {
      sceneVertices[vertexOffset+v] = s.meshes[m].vertices[v];
    }
    for (unsigned int i=0; i<s.meshes[m].numVertexIndices; i++) {
      sceneVertexIndices[vertexIndexOffset+i] = s.meshes[m].vertexIndices[i];
    }
    vertexOffset += s.meshes[m].numVertices;
    vertexIndexOffset += s.meshes[m].numVertexIndices;
  }

  glGenVertexArrays(1, &s.VAO);
  glBindVertexArray(s.VAO);

  glGenBuffers(1, &s.VBO);
  glBindBuffer(GL_ARRAY_BUFFER, s.VBO);
  glBufferData(GL_ARRAY_BUFFER, numSceneVertices * sizeof(vertex), sceneVertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)offsetof(vertex, position));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)offsetof(vertex, normal));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)offsetof(vertex, color));
  glEnableVertexAttribArray(2);

  glGenBuffers(1, &s.EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, numSceneVertexIndices * sizeof(short), sceneVertexIndices, GL_STATIC_DRAW);

  glBindVertexArray(0);

  free(sceneVertices);
  free(sceneVertexIndices);

  return s;
}
