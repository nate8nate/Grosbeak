#ifndef entity_hpp
#define entity_hpp

#include <string.h>
#include "mesh.hpp"

struct entity {
  unsigned int numMeshes;
  vec3 location;
  vec3 scale;
  quat rotation;

  mesh * meshes;
};

const char entityDir[] = "entities/";
const char entityExt[] = ".entity";

entity loadEntity(const char * path);
void draw(entity e);
void destroyEntity(entity e);

#endif
