#include "lights.hpp"

dirLight loadDirLight(const char *path) {
  FILE *dirLightFile = fopen(path, "r");
  dirLight d;

  fscanf(dirLightFile, "%f,%f,%f",
         &d.direction.x,
         &d.direction.y,
         &d.direction.z);
  fscanf(dirLightFile, "%f,%f,%f",
         &d.diffuse.r,
         &d.diffuse.g,
         &d.diffuse.b);

  fclose(dirLightFile);
  return d;
}

pointLight loadPointLight(const char *path) {
  FILE *pointLightFile = fopen(path, "r");
  pointLight p;

  fscanf(pointLightFile, "%f,%f,%f",
         &p.location.x,
         &p.location.y,
         &p.location.z);
  fscanf(pointLightFile, "%f", &p.constant);
  fscanf(pointLightFile, "%f", &p.linear);
  fscanf(pointLightFile, "%f", &p.quadratic);
  fscanf(pointLightFile, "%f,%f,%f",
         &p.diffuse.r,
         &p.diffuse.g,
         &p.diffuse.b);

  fclose(pointLightFile);
  return p;
}