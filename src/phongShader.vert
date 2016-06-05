#version 330 core
out vec3 vNormal;
out vec3 vModelPos;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(position, 1.0f);
  vNormal = mat3(transpose(inverse(model))) * normal; // TODO: calculate Normal Matrix on CPU.
  vModelPos = vec3(model * vec4(position, 1.0f));
}
