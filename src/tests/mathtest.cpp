#if 0
#include <stdio.h>
#include "src/linmath.h"

int main() {
  printf("Theirs\n");
  mat4x4 m;

  vec3 eye = {.2f, .4f, .3f};
  vec3 at = {.1f, .5f, .4f};
  vec3 up = {0.f, 1.f, 0.f};

  mat4x4_look_at(m, eye, at, up);
  printf("Look At:\n");
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      printf("%f ", m[i][j]);
    }
    printf("\n");
  }

  mat4x4_perspective(m, 1.57f, 1.33333f, .1f, 100.f);
  printf("Perspective:\n");
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      printf("%f ", m[i][j]);
    }
    printf("\n");
  }

  mat4x4_translate_in_place(m, 2.f, 3.f, 4.f);
  printf("Translate:\n");
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      printf("%f ", m[i][j]);
    }
    printf("\n");
  }

  float angle = M_PI/8.0f;
  mat4x4_rotate(m, m, 1.0f, 0.3f, 0.5f, angle);
  printf("Rotate:\n");
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      printf("%f ", m[i][j]);
    }
    printf("\n");
  }

  return 0;
}
#else
#include <stdio.h>
#include "src/linear_math.hpp"

int main() {
  printf("Mine\n");
  mat4 m;

  vec3 eye = Vec3(.2f, .4f, .3f);
  vec3 at = Vec3(.1f, .5f, .4f);
  vec3 up = Vec3(0.f, 1.f, 0.f);

  m = lookAt(eye, at, up);
  printf("Look At:\n");
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      printf("%f ", m.A[i][j]);
    }
    printf("\n");
  }

  m = perspective(1.57f, 1.33333f, .1f, 100.f);
  printf("Perspective:\n");
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      printf("%f ", m.A[i][j]);
    }
    printf("\n");
  }

  m = translate(m, 2.f, 3.f, 4.f);
  printf("Tranlate:\n");
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      printf("%f ", m.A[i][j]);
    }
    printf("\n");
  }

  float angle = M_PI/8.0f;
  m = rotate(m, 1.0f, 0.3f, 0.5f, angle);
  printf("Rotate:\n");
  for (int i=0; i<4; ++i) {
    for (int j=0; j<4; ++j) {
      printf("%f ", m.A[i][j]);
    }
    printf("\n");
  }

  return 0;
}
#endif
