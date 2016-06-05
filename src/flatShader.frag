#version 330 core
flat in vec3 vColor;

out vec4 color;

//uniform vec3 objectColor;

void main()
{
  color = vec4(vColor, 1.0f);
}