#include "shader.hpp"

void loadShader(GLuint shader, const char * shaderPath) {
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

GLuint loadVertexShader(const char * vertexPath) {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  loadShader(vertexShader, vertexPath);
  return vertexShader;
}

GLuint loadGeometryShader(const char * geometryPath) {
  GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
  loadShader(geometryShader, geometryPath);
  return geometryShader;
}

GLuint loadFragmentShader(const char * fragmentPath) {
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  loadShader(fragmentShader, fragmentPath);
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

GLuint loadShaders(const char * vertexPath,
                   const char * fragmentPath) {
  GLuint vertexShader = loadVertexShader(vertexPath);
  GLuint fragmentShader = loadFragmentShader(fragmentPath);

  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  linkProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return program;
}

GLuint loadShaders(const char * vertexPath,
                   const char * geometryPath,
                   const char * fragmentPath) {
  GLuint vertexShader = loadVertexShader(vertexPath);
  GLuint geometryShader = loadGeometryShader(geometryPath);
  GLuint fragmentShader = loadFragmentShader(fragmentPath);

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
