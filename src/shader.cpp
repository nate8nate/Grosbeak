#include "shader.hpp"

const char shaderDir[] = "shaders/";

GLint compileShader(GLuint shader, const char *shaderBuffer) {
  GLint success;
  glShaderSource(shader, 1, &shaderBuffer, NULL);
  free((void *)shaderBuffer);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  return success;
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

GLuint loadShaders(const char *shaderName, unsigned int numDirLights, unsigned int numPointLights) {
  GLchar infoLog[512];
  GLuint program = glCreateProgram();
  GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint geomShader = glCreateShader(GL_GEOMETRY_SHADER);
  GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

  char *vertShaderPath = NULL;
  asprintf(&vertShaderPath, "%s%s%s", shaderDir, shaderName, ".vert");
  if (fileExists(vertShaderPath)) {
    char *vertShaderBuffer = NULL;
    const char *preVertShaderBuffer = fileToChars(vertShaderPath);
    asprintf(&vertShaderBuffer, preVertShaderBuffer, numDirLights, numPointLights);
    free((void *)preVertShaderBuffer);

    if (!compileShader(vertShader, vertShaderBuffer)) {
      glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
      fprintf(stderr, "'%s' compilation failed: %s\n", vertShaderPath, infoLog);
    } else {
      glAttachShader(program, vertShader);
    }
    // free((void *)vertShaderBuffer);
  }
  free((void *)vertShaderPath);

  char *geomShaderPath = NULL;
  asprintf(&geomShaderPath, "%s%s%s", shaderDir, shaderName, ".geom");
  if (fileExists(geomShaderPath)) {
    const char *geomShaderBuffer = fileToChars(geomShaderPath);

    if (!compileShader(geomShader, geomShaderBuffer)) {
      glGetShaderInfoLog(geomShader, 512, NULL, infoLog);
      fprintf(stderr, "'%s' compilation failed: %s\n", geomShaderPath, infoLog);
    } else {
      glAttachShader(program, geomShader);
    }
    // free((void *)geomShaderBuffer);
  }
  free((void *)geomShaderPath);

  char *fragShaderPath = NULL;
  asprintf(&fragShaderPath, "%s%s%s", shaderDir, shaderName, ".frag");
  if (fileExists(fragShaderPath)) {
    const char *fragShaderBuffer = fileToChars(fragShaderPath);

    if (!compileShader(fragShader, fragShaderBuffer)) {
      glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
      fprintf(stderr, "'%s' compilation failed: %s\n", fragShaderPath, infoLog);
    } else {
      glAttachShader(program, fragShader);
    }
    // free((void *)fragShaderBuffer);
  }
  free((void *)fragShaderPath);

  linkProgram(program);
  glDeleteShader(vertShader);
  glDeleteShader(geomShader);
  glDeleteShader(fragShader);

  return program;
}
