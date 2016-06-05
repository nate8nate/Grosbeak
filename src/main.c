#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <OpenGl/gl3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "linmath.h"

#include "shader.h"

const GLuint WIDTH = 800, HEIGHT = 600;

int main(void) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_Window *window = SDL_CreateWindow(
    "Game3",
    SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
    WIDTH,HEIGHT,
    SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1");
  SDL_SetRelativeMouseMode(SDL_TRUE);

  SDL_GL_SetSwapInterval(0);

  glViewport(0,0, WIDTH, HEIGHT);
  glEnable(GL_DEPTH_TEST);
//  glEnable(GL_CULL_FACE);

//  GLuint lightingSP = loadShaders("phongShader.vert", "phongShader.frag");
//  GLuint lightingSP = loadShaders("gouraudShader.vert", "gouraudShader.frag");
  GLuint lightingSP = loadShaders("flatShader.vert", "flatShader.frag");
  GLuint lightObjectSP = loadShaders("lampshader.vert", "lampshader.frag");

  GLuint matAmbientLoc = glGetUniformLocation(lightingSP, "material.ambient");
  GLuint matDiffuseLoc = glGetUniformLocation(lightingSP, "material.diffuse");
  GLuint matSpecularLoc = glGetUniformLocation(lightingSP, "material.specular");
  GLuint matShineLoc = glGetUniformLocation(lightingSP, "material.shininess");

  GLuint lightAmbientLoc = glGetUniformLocation(lightingSP, "light.ambient");
  GLuint lightDiffuseLoc = glGetUniformLocation(lightingSP, "light.diffuse");
  GLuint lightSpecularLoc = glGetUniformLocation(lightingSP, "light.specular");

  GLfloat vertices[] = {
    // Positions          // Normals
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };

  vec3 cubePositions[] = {
    { 0.0f,  0.0f,  0.0f},
    { 2.0f,  5.0f, -15.0f},
    {-1.5f, -2.2f, -2.5f},
    {-3.8f, -2.0f, -12.3f},
    { 2.4f, -0.4f, -3.5f},
    {-1.7f,  3.0f, -7.5f},
    { 1.3f, -2.0f, -2.5f},
    { 1.5f,  2.0f, -2.5f},
    { 1.5f,  0.2f, -1.5f},
    {-1.3f,  1.0f, -1.5f}
  };

  vec3 pointLightPositions[] = {
    { 0.7f,  0.2f,  2.0f},
    { 2.3f, -3.3f, -4.0f},
    {-4.0f,  2.0f, -12.0f},
    { 0.0f,  0.0f, -3.0f}
  };

  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  GLuint cubeVAO;
  glGenVertexArrays(1, &cubeVAO);
  glBindVertexArray(cubeVAO);

  // Positions
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);
  // Normals
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  glBindVertexArray(0);

  GLuint lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  mat4x4 model;
  mat4x4 view;
  mat4x4 projection;
  vec3 cameraPos = {0.0f, 0.0f, -3.0f};
  vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
  vec3 cameraFront = {0.0f, 0.0f,-1.0f};
  vec3 cameraUp = {0.0f, 1.0f, 0.0f};
  vec3 cameraRight = {-1.0f, 0.0f, 0.0f};
  float pitch=0.0f, yaw=0.0f;
  float fov = 45.0f;

  SDL_Event event;
  Uint32 last=0, dt=0;
  double dts = 0.0;
  unsigned int frames = 0, updateTitle = 0;
  char title[32];

  const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

  int mousedx,mousedy;
  int mouseWheeldy;
  double msensitivity = 0.005;

  // ------- THE MAIN LOOP -------
  int running = 1;
  while (running) {
    mousedx=0,mousedy=0,mouseWheeldy=0;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = 0;
      }
      else if (event.type == SDL_MOUSEMOTION) {
        mousedx = event.motion.xrel;
        mousedy = event.motion.yrel;
      }
      else if (event.type == SDL_MOUSEWHEEL) {
        mouseWheeldy = event.wheel.y;
      }
    }

    Uint32 now = SDL_GetTicks();
    dt = now-last;
    last = now;
    dts = dt/1000.0;
    float velocityMultiplier;

    updateTitle += dt;
    frames++;
    if (updateTitle >= 1000) {
      double d = (updateTitle/(double)frames);
      snprintf(title, sizeof(title), "FPS: %d (%f)", 1000*frames/updateTitle, d);
      SDL_SetWindowTitle(window, title);
      updateTitle = 0;
      frames = 0;
    }

    if (keyboardState[SDL_SCANCODE_ESCAPE]) {
      SDL_Event quit;
      quit.type = SDL_QUIT;
      SDL_PushEvent(&quit);
    }
    if (keyboardState[SDL_SCANCODE_LSHIFT]) {
      velocityMultiplier = 20.0f;
    } else {
      velocityMultiplier = 5.0f;
    }
    if (keyboardState[SDL_SCANCODE_W]) {
      vec3 dLoc;
      vec3_scale(dLoc, cameraFront, dts*velocityMultiplier);
      vec3_add(cameraPos, cameraPos, dLoc);
    }
    if (keyboardState[SDL_SCANCODE_S]) {
      vec3 dLoc;
      vec3_scale(dLoc, cameraFront, dts*velocityMultiplier);
      vec3_sub(cameraPos, cameraPos, dLoc);
    }
    if (keyboardState[SDL_SCANCODE_A]) {
      vec3 dLoc;
      vec3_scale(dLoc, cameraRight, dts*velocityMultiplier);
      vec3_sub(cameraPos, cameraPos, dLoc);
    }
    if (keyboardState[SDL_SCANCODE_D]) {
      vec3 dLoc;
      vec3_scale(dLoc, cameraRight, dts*velocityMultiplier);
      vec3_add(cameraPos, cameraPos, dLoc);
    }

    yaw += mousedx * msensitivity;
    pitch -= mousedy * msensitivity;
    if (pitch > M_PI/2.1f) {
      pitch = M_PI/2.1f;
    }
    if (pitch < -M_PI/2.1f) {
      pitch = -M_PI/2.1f;
    }

    cameraFront[0] = cos(pitch) * cos(yaw);
    cameraFront[1] = sin(pitch);
    cameraFront[2] = cos(pitch) * sin(yaw);

    fov -= (float)mouseWheeldy/100.0f;
    if (fov <= M_PI/16.0f) {
      fov = M_PI/16.0f;
    }
    else if (fov >= M_PI_2) {
      fov = M_PI_2;
    }

    vec3_mul_cross(cameraRight, cameraFront, cameraUp);
    vec3_norm(cameraRight, cameraRight);

    vec3_add(cameraTarget, cameraPos, cameraFront);
    mat4x4_look_at(view, cameraPos, cameraTarget, cameraUp);
    mat4x4_perspective(projection, fov, WIDTH/(float)HEIGHT, 0.1f, 100.0f);

    glClearColor(0.4f, 0.15f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glUseProgram(lightingSP);

    // Directional light
    glUniform3f(glGetUniformLocation(lightingSP, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
    glUniform3f(glGetUniformLocation(lightingSP, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(lightingSP, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
    glUniform3f(glGetUniformLocation(lightingSP, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
    // Point light 1
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[0].position"), pointLightPositions[0][0], pointLightPositions[0][1], pointLightPositions[0][2]);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[0].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[0].linear"), 0.09);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[0].quadratic"), 0.032);
    // Point light 2
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[1].position"), pointLightPositions[1][0], pointLightPositions[1][1], pointLightPositions[1][2]);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[1].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[1].linear"), 0.09);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[1].quadratic"), 0.032);
    // Point light 3
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[2].position"), pointLightPositions[2][0], pointLightPositions[2][1], pointLightPositions[2][2]);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[2].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[2].linear"), 0.09);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[2].quadratic"), 0.032);
    // Point light 4
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[3].position"), pointLightPositions[3][0], pointLightPositions[3][1], pointLightPositions[3][2]);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
    glUniform3f(glGetUniformLocation(lightingSP, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[3].constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[3].linear"), 0.09);
    glUniform1f(glGetUniformLocation(lightingSP, "pointLights[3].quadratic"), 0.032);
    // SpotLight
    glUniform3f(glGetUniformLocation(lightingSP, "spotLight.position"), cameraPos[0], cameraPos[1], cameraPos[2]);
    glUniform3f(glGetUniformLocation(lightingSP, "spotLight.direction"), cameraFront[0], cameraFront[1], cameraFront[2]);
    glUniform3f(glGetUniformLocation(lightingSP, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(lightingSP, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(lightingSP, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(lightingSP, "spotLight.constant"), 1.0f);
    glUniform1f(glGetUniformLocation(lightingSP, "spotLight.linear"), 0.09);
    glUniform1f(glGetUniformLocation(lightingSP, "spotLight.quadratic"), 0.032);
    glUniform1f(glGetUniformLocation(lightingSP, "spotLight.cutOff"), cos(.2));
    glUniform1f(glGetUniformLocation(lightingSP, "spotLight.outerCutOff"), cos(.25));

    GLuint cameraPosLoc = glGetUniformLocation(lightingSP, "viewPos");
    glUniform3f(cameraPosLoc, cameraPos[0], cameraPos[1], cameraPos[2]);

    GLuint modelLoc = glGetUniformLocation(lightingSP, "model");
    GLuint viewLoc = glGetUniformLocation(lightingSP, "view");
    GLuint projectionLoc = glGetUniformLocation(lightingSP, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat*)view);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat*)projection);
    glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
    glUniform3f(matSpecularLoc, 0.1f, 0.1f, 0.1f);
    glUniform1f(matShineLoc, 8.0f);
    glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
    glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
    glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

    glBindVertexArray(cubeVAO);
    for(GLuint i = 0; i < 10; i++) {
      mat4x4_identity(model);
      mat4x4_translate(model, cubePositions[i][0], cubePositions[i][1], cubePositions[i][2]);
      GLfloat angle = M_PI/8.0f * i;
      mat4x4_rotate(model, model, 1.0f, 0.3f, 0.5f, angle);
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat*)model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    glUseProgram(lightObjectSP);
    modelLoc = glGetUniformLocation(lightObjectSP, "model");
    viewLoc = glGetUniformLocation(lightObjectSP, "view");
    projectionLoc = glGetUniformLocation(lightObjectSP, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat*)view);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat*)projection);

    glBindVertexArray(lightVAO);
    for (GLuint i=0; i<4; i++) {
      mat4x4_identity(model);
      mat4x4_translate(model, pointLightPositions[i][0], pointLightPositions[i][1], pointLightPositions[i][2]);
      mat4x4_scale_aniso(model, model, 0.2f, 0.2f, 0.2f);
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat*)model);
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);

    SDL_GL_SwapWindow(window);
  }

  glDeleteProgram(lightingSP);
  glDeleteProgram(lightObjectSP);

  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &VBO);

  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
