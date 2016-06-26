#version 330 core
struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

uniform Material material;

out vec4 fColor;

void main() {
  fColor = vec4(material.diffuse, 1.0f);
}