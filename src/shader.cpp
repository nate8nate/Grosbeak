#include "shader.hpp"

const char shaderDir[] = "shaders/";

void loadShader(GLuint shader, const char *shaderName, const char *shaderExt) {
  char shaderPath[128];
  strcpy(shaderPath, shaderDir);
  strcat(shaderPath, shaderName);
  strcat(shaderPath, shaderExt);
  const GLchar * shaderFile = fileToString(shaderPath);
  GLint success;
  GLchar infoLog[512];

  glShaderSource(shader, 1, &shaderFile, NULL);
  free((void *)shaderFile);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    fprintf(stderr, "'%s' compilation failed: %s\n", shaderPath, infoLog);
  }
}

GLuint loadVertexShader(const char *vertexName) {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  loadShader(vertexShader, vertexName, ".vert");
  return vertexShader;
}

GLuint loadGeometryShader(const char *geometryName) {
  GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  loadShader(geometryShader, geometryName, ".geom");
  return geometryShader;
}

GLuint loadFragmentShader(const char *fragmentName) {
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  loadShader(fragmentShader, fragmentName, ".frag");
  return fragmentShader;
}

void linkProgram(GLuint program) {
  glLinkProgram(program);
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    fprintf(stderr, "Shader linking failed: %s\n", infoLog);
  }
}

GLuint loadShadersFV(const char *shaderName) {
  GLuint vertexShader = loadVertexShader(shaderName);
  GLuint fragmentShader = loadFragmentShader(shaderName);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  linkProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

GLuint loadShadersFGV(const char *shaderName) {
  GLuint vertexShader = loadVertexShader(shaderName);
  GLuint geometryShader = loadGeometryShader(shaderName);
  GLuint fragmentShader = loadFragmentShader(shaderName);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, geometryShader);
  glAttachShader(program, fragmentShader);
  linkProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(geometryShader);
  glDeleteShader(fragmentShader);

  return program;
}
