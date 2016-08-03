#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <OpenGl/gl3.h>

#include "linear_math.hpp"
#include "scene.hpp"
#include "shader.hpp"

int main(void) {
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  SDL_Window *window = SDL_CreateWindow(
    "Game3",
    SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
    800,600,
    SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);

  SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1");
  SDL_SetRelativeMouseMode(SDL_TRUE);

#if 0
  SDL_GL_SetSwapInterval(-1);
#else
  SDL_GL_SetSwapInterval(0);
#endif

  int width, height;
  SDL_GetWindowSize(window, &width, &height);

  glViewport(0,0, width, height);
  glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);

//   GLuint lightingSP = loadShadersFV("gouraud");
// //  GLuint lightingSP = loadShaders("shaders/flat.vert", "shaders/flat.frag");
// //  GLuint lightingSP = loadShaders("shaders/shadeless.vert", "shaders/shadeless.frag");
//   GLuint normalSP = loadShadersFGV("normal");
//   GLuint lightObjectSP = loadShadersFV("shadeless");

  // GLint matAmbientLoc = glGetUniformLocation(lightingSP, "material.ambient");
  // GLint matDiffuseLoc = glGetUniformLocation(lightingSP, "material.diffuse");
  // GLint matSpecularLoc = glGetUniformLocation(lightingSP, "material.specular");
  // GLint matShineLoc = glGetUniformLocation(lightingSP, "material.shininess");

  // GLint lightAmbientLoc = glGetUniformLocation(lightingSP, "light.ambient");
  // GLint lightDiffuseLoc = glGetUniformLocation(lightingSP, "light.diffuse");
  // GLint lightSpecularLoc = glGetUniformLocation(lightingSP, "light.specular");

  scene s = loadScene("Scene");

  player p;
  p.location = Vec3(0.f, 0.f, 0.f);
  p.heading = Vec3(0.f, 0.f, -1.f);
  p.up = Vec3(0.f, 1.f, 0.f);

  float fov = 45.0f;
  float pitch = 0.0f, yaw = 0.0f;

  // GLuint VAO;
  // glGenVertexArrays(1, &VAO);
  // glBindVertexArray(VAO);
  //
  // GLuint VBO;
  // glGenBuffers(1, &VBO);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, d.mesh.numVertices*sizeof(vertex), d.mesh.vertices, GL_STATIC_DRAW);
  //
  // // Positions
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)offsetof(vertex, position));
  // glEnableVertexAttribArray(0);
  // // Normals
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)offsetof(vertex, normal));
  // glEnableVertexAttribArray(1);
  // // Colors
  // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid *)offsetof(vertex, color));
  // glEnableVertexAttribArray(2);
  //
  // GLuint EBO;
  // glGenBuffers(1, &EBO);
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // glBufferData(GL_ELEMENT_ARRAY_BUFFER, d.mesh.numIndices*sizeof(unsigned short), d.mesh.indices, GL_STATIC_DRAW);
  //
  // glBindVertexArray(0);

//  GLuint lightVAO;
//  glGenVertexArrays(1, &lightVAO);
//  glBindVertexArray(lightVAO);
//  glBindBuffer(GL_ARRAY_BUFFER, VBO);
//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
//  glEnableVertexAttribArray(0);
//  glBindVertexArray(0);

  // mat4 model;
  // mat4 view;
  // mat4 projection;

  SDL_Event event;
  Uint32 last=0, dt=0;
  double dts = 0.0;
  unsigned int frames = 0, updateTitle = 0;
  char title[32];

  const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

  int mousedx,mousedy;
  int mouseWheeldy;
  double msensitivity = 0.005;

  // ------ THE MAIN LOOP ------
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
      double delta = (updateTitle/(double)frames);
      snprintf(title, sizeof(title), "FPS: %d (%f)", 1000*frames/updateTitle, delta);
      SDL_SetWindowTitle(window, title);
      updateTitle = 0;
      frames = 0;
    }

    yaw += mousedx * msensitivity;
    pitch -= mousedy * msensitivity;
    if (pitch > M_PI/2.1f) {
      pitch = M_PI/2.1;
    }
    if (pitch < -M_PI/2.1f) {
      pitch = -M_PI/2.1f;
    }

    // vec3 cameraPos = Vec3(0.0f, 0.0f, -3.0f);
    // vec3 cameraFront = Vec3(0.0f, 0.0f,-1.0f);
    // vec3 cameraUp = Vec3(0.0f, 1.0f, 0.0f);
    // vec3 cameraRight = normalize( cross(cameraFront, cameraUp));

    // cameraFront.x = cosf(pitch) * cosf(yaw);
    // cameraFront.y = sinf(pitch);
    // cameraFront.z = cosf(pitch) * sinf(yaw);

    p.heading = Vec3(cosf(pitch) * cosf(yaw), sinf(pitch), cosf(pitch) * sinf(yaw));
    vec3 playerRight = normalize( cross(p.heading, p.up));

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
      vec3 dLoc = (dts*velocityMultiplier) * p.heading;
      p.location = p.location + dLoc;
    }
    if (keyboardState[SDL_SCANCODE_S]) {
      vec3 dLoc = (dts*velocityMultiplier) * p.heading;
      p.location = p.location - dLoc;
    }
    if (keyboardState[SDL_SCANCODE_A]) {
      vec3 dLoc = (dts*velocityMultiplier) * playerRight;
      p.location = p.location - dLoc;
    }
    if (keyboardState[SDL_SCANCODE_D]) {
      vec3 dLoc = (dts*velocityMultiplier) * playerRight;
      p.location = p.location + dLoc;
    }

    fov -= (float)mouseWheeldy/100.0f;
    if (fov <= M_PI/16.0f) {
      fov = M_PI/16.0f;
    }
    else if (fov >= M_PI_2) {
      fov = M_PI_2;
    }

    // cameraTarget = cameraPos + cameraFront;
    // view = translation(-cameraPos) * lookAt(cameraPos, cameraTarget, cameraUp);

    glClearColor(s.sky.r, s.sky.g, s.sky.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    mat4 projection = perspective(fov, (float)width/height, 0.1f, 100.0f);
    drawScene(s, p, projection);

    // glUseProgram(s.matShader.ID);

    // Directional light
    // glUniform3f(glGetUniformLocation(lightingSP, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
    // glUniform3f(glGetUniformLocation(lightingSP, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
    // glUniform3f(glGetUniformLocation(lightingSP, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
    // glUniform3f(glGetUniformLocation(lightingSP, "dirLight.specular"), 0.5f, 0.5f, 0.5f);
    // // Point light 1
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[0].constant"), 1.0f);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[0].linear"), 0.09);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[0].quadratic"), 0.032);
    // // Point light 2
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[1].diffuse"), 0.8f, 0.8f, 0.8f);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[1].constant"), 1.0f);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[1].linear"), 0.09);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[1].quadratic"), 0.032);
    // // Point light 3
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[2].ambient"), 0.05f, 0.05f, 0.05f);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[2].diffuse"), 0.8f, 0.8f, 0.8f);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[2].constant"), 1.0f);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[2].linear"), 0.09);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[2].quadratic"), 0.032);
    // // Point light 4
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[3].ambient"), 0.05f, 0.05f, 0.05f);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[3].diffuse"), 0.8f, 0.8f, 0.8f);
    // glUniform3f(glGetUniformLocation(lightingSP, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[3].constant"), 1.0f);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[3].linear"), 0.09);
    // glUniform1f(glGetUniformLocation(lightingSP, "pointLights[3].quadratic"), 0.032);

    // GLint cameraPosLoc = glGetUniformLocation(lightingSP, "viewPos");
    // glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
    //
    // GLint modelLoc = glGetUniformLocation(lightingSP, "model");
    // GLint normalMatrixLoc = glGetUniformLocation(lightingSP, "normalMatrix");
    // GLint viewLoc = glGetUniformLocation(lightingSP, "view");
    // GLint projectionLoc = glGetUniformLocation(lightingSP, "projection");
    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat *)view.flat);
    // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat *)projection.flat);
    // glUniform3f(matAmbientLoc, 1.0f, 0.5f, 0.31f);
    // glUniform3f(matDiffuseLoc, 1.0f, 0.5f, 0.31f);
    // glUniform3f(matSpecularLoc, 0.1f, 0.1f, 0.1f);
    // glUniform1f(matShineLoc, 8.0f);
    // glUniform3f(lightAmbientLoc, 0.2f, 0.2f, 0.2f);
    // glUniform3f(lightDiffuseLoc, 0.5f, 0.5f, 0.5f);
    // glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

    // glBindVertexArray(VAO);
    // mat4 normalMatrix;
    // model = translation(d.location);
    // model = rotate(model, d.rotation);
    // model = scale(model, d.scale);
    // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.flat);
    // normalMatrix = transpose(inverse(model));
    // glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix.flat);
    // glDrawElements(GL_TRIANGLES, d.mesh.numIndices, GL_UNSIGNED_SHORT, (void *)0);
    // glBindVertexArray(0);
    //
    // glBindVertexArray(VAO);
    // glUseProgram(normalSP);
    // modelLoc = glGetUniformLocation(normalSP, "model");
    // normalMatrixLoc = glGetUniformLocation(normalSP, "normalMatrix");
    // viewLoc = glGetUniformLocation(normalSP, "view");
    // projectionLoc = glGetUniformLocation(normalSP, "projection");
    // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.flat);
    // glUniformMatrix4fv(normalMatrixLoc, 1, GL_FALSE, normalMatrix.flat);
    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.flat);
    // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.flat);
    // glDrawElements(GL_POINTS, d.mesh.numIndices, GL_UNSIGNED_SHORT, 0);
    // glBindVertexArray(0);
    //
    // glUseProgram(lightObjectSP);
    // modelLoc = glGetUniformLocation(lightObjectSP, "model");
    // viewLoc = glGetUniformLocation(lightObjectSP, "view");
    // projectionLoc = glGetUniformLocation(lightObjectSP, "projection");
    // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat *)view.flat);
    // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat *)projection.flat);
    //
    // glBindVertexArray(VAO);
    // for (GLuint i=0; i<4; ++i) {
    //   model = translation(pointLightPositions[i]);
    //   model = scale(model, .2f);
    //   glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat *)model.flat);
    //   glDrawElements(GL_TRIANGLES, d.mesh.numIndices, GL_UNSIGNED_SHORT, (void *)0);
    // }
    // glBindVertexArray(0);

    SDL_GL_SwapWindow(window);
  }

  // destroyEntity(e);
  //
  // glDeleteProgram(lightingSP);
  // glDeleteProgram(lightObjectSP);
  //
  // glDeleteBuffers(1, &EBO);
  // glDeleteBuffers(1, &VBO);
  // glDeleteVertexArrays(1, &VAO);
  deleteScene(s);

  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
