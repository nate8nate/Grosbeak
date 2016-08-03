#include "scene.hpp"

const char sceneDir[] = "scenes/";
const char meshDir[] = "meshes/";

const char sceneExt[] = ".scene";
const char meshExt[] = ".mesh";

void drawScene(scene s, player p, mat4 projection) {
  glUseProgram(s.matShader.ID);

  glUniform3f(s.matShader.worldAmbientLoc, s.ambient.r, s.ambient.g, s.ambient.b);
  glUniform3f(s.matShader.matDiffuseLoc, .4f, .2f, .2f);
  glUniform3f(s.matShader.matSpecularLoc, .2f, .5f, .1f);
  glUniform1f(s.matShader.matHardnessLoc, 32);

  // TODO preprocess values => arrays
  for (unsigned int d=0; d<s.numDirLights; d++) {
    glUniform3fv(s.matShader.dirLightDirectionLocs[d], 1, s.dirLights[d].direction.A);
    glUniform3fv(s.matShader.dirLightDiffuseLocs[d], 1, s.dirLights[d].diffuse.A);
  }
  for (unsigned int p=0; p<s.numPointLights; p++) {
    glUniform3fv(s.matShader.pointLightLocationLocs[p], 1, s.pointLights[p].location.A);
    glUniform3fv(s.matShader.pointLightDiffuseLocs[p], 1, s.pointLights[p].diffuse.A);
    glUniform1f(s.matShader.pointLightConstantLocs[p], s.pointLights[p].constant);
    glUniform1f(s.matShader.pointLightLinearLocs[p], s.pointLights[p].linear);
    glUniform1f(s.matShader.pointLightQuadraticLocs[p], s.pointLights[p].quadratic);
  }

  glUniform3fv(s.matShader.viewPosLoc, 1, p.location.A);
  mat4 view = translation(-p.location) * lookAt(p.location, p.location + p.heading, p.up);
  glUniformMatrix4fv(s.matShader.viewLoc, 1, GL_FALSE, view.A);
  glUniformMatrix4fv(s.matShader.projectionLoc, 1, GL_FALSE, projection.A);

  for (unsigned int e=0; e<s.numEntities; e++) {
    mat4 model = scale( rotate( translation(s.entities[e].location), s.entities[e].rotation), s.entities[e].scale);
    mat4 normalMatrix = transpose( inverse(model));
    glUniformMatrix4fv(s.matShader.modelLoc, 1, GL_FALSE, model.A);
    glUniformMatrix4fv(s.matShader.normalMatrixLoc, 1, GL_FALSE, normalMatrix.A);

    glBindVertexArray(s.VAO);
    glDrawElements(GL_TRIANGLES, s.meshes[s.entities[e].meshIndex].numVertexIndices, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
  }
}

scene loadScene(const char *sceneName) {
  char *scenePath;
  asprintf(&scenePath, "%s%s%s", sceneDir, sceneName, sceneExt);
  FILE * sceneFile = fopen(scenePath, "r");
  free(scenePath);
  scene s;

  unsigned int numSceneVertices = 0;
  unsigned int numSceneVertexIndices = 0;

  fscanf(sceneFile, "%u", &s.numMeshes);
  s.meshes = (mesh *)malloc(s.numMeshes * sizeof(mesh));
  for (unsigned int m=0; m<s.numMeshes; m++) {
    mesh msh;

    char meshName[64];
    fscanf(sceneFile, " %64s", meshName);
    char *meshPath;
    asprintf(&meshPath, "%s%s%s", meshDir, meshName, meshExt);
    FILE *meshFile = fopen(meshPath, "r");
    free(meshPath);

    fscanf(meshFile, "%u", &msh.numVertices);
    msh.vertices = (vertex *)malloc(msh.numVertices * sizeof(vertex));
    for (unsigned int v=0; v<msh.numVertices; v++) {
      vertex vert;
      fscanf(meshFile, "%f,%f,%f\t%f,%f,%f\t%f,%f,%f",
                        &vert.position.x, &vert.position.y, &vert.position.z,
                        &vert.normal.x, &vert.normal.y, &vert.normal.z,
                        &vert.color.r, &vert.color.g, &vert.color.b);
      msh.vertices[v] = vert;
    }
    fscanf(meshFile, "%u", &msh.numVertexIndices);
    msh.vertexIndices = (unsigned short *)malloc(msh.numVertexIndices * sizeof(short));
    for (unsigned int i=0; i<msh.numVertexIndices; i=i+3) {
      fscanf(meshFile, "%hu\t%hu\t%hu",
                        &msh.vertexIndices[i],
                        &msh.vertexIndices[i+1],
                        &msh.vertexIndices[i+2]);
    }

    numSceneVertices += msh.numVertices;
    numSceneVertexIndices += msh.numVertexIndices;

    fclose(meshFile);
    s.meshes[m] = msh;
  }

  fscanf(sceneFile, "%u", &s.numEntities);
  s.entities = (entity *)malloc(s.numEntities * sizeof(entity));
  for (unsigned int e=0; e<s.numEntities; e++) {
    entity ent;
    fscanf(sceneFile, "%u", &ent.meshIndex);
    fscanf(sceneFile, "%f,%f,%f", &ent.location.x, &ent.location.y, &ent.location.z);
    fscanf(sceneFile, "%f,%f,%f,%f", &ent.rotation.r, &ent.rotation.i, &ent.rotation.j, &ent.rotation.k);
    fscanf(sceneFile, "%f,%f,%f", &ent.scale.x, &ent.scale.y, &ent.scale.z);
    s.entities[e] = ent;
  }

  fscanf(sceneFile, "%u", &s.numDirLights);
  s.dirLights = (directionalLight *)malloc(s.numDirLights * sizeof(directionalLight));
  for (unsigned int d=0; d<s.numDirLights; d++) {
    directionalLight dir;
    fscanf(sceneFile, "%f,%f,%f", &dir.direction.x, &dir.direction.y, &dir.direction.z);
    fscanf(sceneFile, "%f,%f,%f", &dir.diffuse.r, &dir.diffuse.g, &dir.diffuse.b);
    s.dirLights[d] = dir;
  }

  fscanf(sceneFile, "%u", &s.numPointLights);
  s.pointLights = (pointLight *)malloc(s.numPointLights * sizeof(pointLight));
  for (unsigned int p=0; p<s.numPointLights; p++)
  {
    pointLight point;
    fscanf(sceneFile, "%f,%f,%f", &point.location.x, &point.location.y, &point.location.z);
    fscanf(sceneFile, "%f", &point.constant);
    fscanf(sceneFile, "%f", &point.linear);
    fscanf(sceneFile, "%f", &point.quadratic);
    fscanf(sceneFile, "%f,%f,%f", &point.diffuse.r, &point.diffuse.g, &point.diffuse.b);
    s.pointLights[p] = point;
  }

  fscanf(sceneFile, "%f,%f,%f", &s.sky.r, &s.sky.g, &s.sky.b);
  fscanf(sceneFile, "%f,%f,%f", &s.ambient.r, &s.ambient.g, &s.ambient.b);

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

  {
    materialShader m;

    char locString[32];

    m.ID = loadShaders("gouraud", s.numDirLights, s.numPointLights);

    m.viewPosLoc = glGetUniformLocation(m.ID, "viewPos");
    m.modelLoc = glGetUniformLocation(m.ID, "model");
    m.normalMatrixLoc = glGetUniformLocation(m.ID, "normalMatrix");
    m.viewLoc = glGetUniformLocation(m.ID, "view");
    m.projectionLoc = glGetUniformLocation(m.ID, "projection");

    m.worldAmbientLoc = glGetUniformLocation(m.ID, "worldAmbient");
    m.matDiffuseLoc = glGetUniformLocation(m.ID, "material.diffuse");
    m.matSpecularLoc = glGetUniformLocation(m.ID, "material.specular");
    m.matHardnessLoc = glGetUniformLocation(m.ID, "material.hardness");

    m.dirLightDirectionLocs = (GLint *)malloc(s.numDirLights * sizeof(GLint));
    m.dirLightDiffuseLocs = (GLint *)malloc(s.numDirLights * sizeof(GLint));
    for (unsigned int d = 0; d<s.numDirLights; d++) {
      sprintf(locString, "dirlights[%i].direction", d);
      m.dirLightDirectionLocs[d] = glGetUniformLocation(m.ID, locString);
      sprintf(locString, "dirlights[%i].diffuse", d);
      m.dirLightDiffuseLocs[d] = glGetUniformLocation(m.ID, locString);
    }

    m.pointLightLocationLocs = (GLint *)malloc(s.numPointLights * sizeof(GLint));
    m.pointLightDiffuseLocs = (GLint *)malloc(s.numPointLights * sizeof(GLint));
    m.pointLightConstantLocs = (GLint *)malloc(s.numPointLights * sizeof(GLint));
    m.pointLightLinearLocs = (GLint *)malloc(s.numPointLights * sizeof(GLint));
    m.pointLightQuadraticLocs = (GLint *)malloc(s.numPointLights * sizeof(GLint));
    for (unsigned int p = 0; p<s.numPointLights; p++) {
      sprintf(locString, "pointLights[%i].location", p);
      m.pointLightLocationLocs[p] = glGetUniformLocation(m.ID, locString);
      sprintf(locString, "pointLights[%i].diffuse", p);
      m.pointLightDiffuseLocs[p] = glGetUniformLocation(m.ID, locString);
      sprintf(locString, "pointLights[%i].constant", p);
      m.pointLightConstantLocs[p] = glGetUniformLocation(m.ID, locString);
      sprintf(locString, "pointLights[%i].linear", p);
      m.pointLightLinearLocs[p] = glGetUniformLocation(m.ID, locString);
      sprintf(locString, "pointLights[%i].quadratic", p);
      m.pointLightQuadraticLocs[p] = glGetUniformLocation(m.ID, locString);
    }

    s.matShader = m;
  }

  return s;
}

void deleteScene(scene s) {
  glDeleteProgram(s.matShader.ID);

  glDeleteBuffers(1, &s.EBO);
  glDeleteBuffers(1, &s.VBO);
  glDeleteVertexArrays(1, &s.VAO);

  for (unsigned int m=0; m<s.numMeshes; m++) {
    free(s.meshes[m].vertices);
    free(s.meshes[m].vertexIndices);
  }

  free(s.matShader.dirLightDirectionLocs);
  free(s.matShader.dirLightDiffuseLocs);

  free(s.matShader.pointLightLocationLocs);
  free(s.matShader.pointLightDiffuseLocs);
  free(s.matShader.pointLightConstantLocs);
  free(s.matShader.pointLightLinearLocs);
  free(s.matShader.pointLightQuadraticLocs);

  free(s.meshes);
  free(s.entities);
  free(s.dirLights);
  free(s.pointLights);
}
