#include "entity.hpp"

void draw(entity e) {
  mat4 model;
  model = translation(e.location);
  model = rotate(model, e.rotation);
  model = scale(model, e.scale);
  for (unsigned int i=0; i<e.numMeshes; ++i) {
    draw(e.meshes[i], model);
  }
}

entity loadEntity(const char *path) {
  FILE *entityFile = fopen(path, "r");
  entity e;

  fscanf(entityFile, "%i", &e.numMeshes);
  for (unsigned int m=0; m<e.numMeshes; ++m) {
    char entityName[64];
    fscanf(entityFile, " %s64", entityName);
    char entityPath[128];
    strcpy(entityPath, entityDir);
    strcat(entityPath, entityName);
    strcat(entityPath, entityExt);
  }

  fscanf(entityFile, "%f,%f,%f",
         &e.location.x,
         &e.location.y,
         &e.location.z);
  fscanf(entityFile, "%f,%f,%f,%f",
         &e.rotation.r,
         &e.rotation.i,
         &e.rotation.j,
         &e.rotation.k);
  fscanf(entityFile, "%f,%f,%f",
         &e.scale.x,
         &e.scale.y,
         &e.scale.z);

  fclose(entityFile);
  return e;
}

void destroyEntity(entity e) {
  for (unsigned int i=0; i<e.numMeshes; ++i) {
    destroyMesh(e.meshes[i]);
  }
}
