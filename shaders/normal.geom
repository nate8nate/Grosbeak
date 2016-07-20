#version 330 core
layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform mat4 model;
uniform mat4 normalMatrix;
uniform mat4 view;
uniform mat4 projection;

uniform float length = .1f;
uniform vec3 color = vec3(1.f, 1.f, 1.f);

in vec3 vNormal[];

out vec3 gColor;

void main() {
  vec3 normal = vNormal[0];
  gColor = normal;

  vec4 v0 = gl_in[0].gl_Position;
  gl_Position = projection * view * model * v0;
  EmitVertex();

  vec4 v1 = v0 + vec4(normal * length, 0.f);
  gl_Position = projection * view * model * v1;
  EmitVertex();
  EndPrimitive(); 
}