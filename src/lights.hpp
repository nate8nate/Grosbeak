#ifndef lights_hpp
#define lights_hpp

#include <stdio.h>
#include "linear_math.hpp"

struct dirLight {
  vec3 direction;
  vec3 diffuse;
};

struct pointLight {
  float constant;
  float linear;
  float quadratic;
  vec3 location;
  vec3 diffuse;
};

const char lightDir[] = "lights/";
const char dirLightExt[] = ".dir";
const char pointLightExt[] = ".point";

dirLight loadDirLight(const char *path);
pointLight loadPointLight(const char *path);

#endif
