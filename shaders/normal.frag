#version 330 core
out vec4 color;

in vec3 gColor;

void main() {
  color = vec4(gColor, 1.f);
}