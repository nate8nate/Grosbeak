#version 330 core
struct Material {
  vec3 specular;
  float shininess;
};

struct DirLight {
  vec3 direction;
  vec3 diffuse;
};

struct PointLight {
  vec3 position;
  vec3 diffuse;
  float constant;
  float linear;
  float quadratic;
};

#define MAX_DIR_LIGHTS 2
#define MAX_POINT_LIGHTS 8

out vec3 vColor;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 worldAmbient;

uniform uint numDirLights;
uniform uint numPointLights;
uniform DirLight dirLights[MAX_DIR_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform Material material;

uniform mat4 model;
uniform mat4 normalMatrix;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0f);
  
  vec3 result;
  vec3 normal = normalize(vec3(normalMatrix * vec4(normal, 1.f)));
  vec3 modelPos = vec3(model * vec4(position, 1.0f));
  vec3 viewDir = normalize(viewPos - modelPos);
  
  // Directional Light
  for (uint i=0; i<numDirLights; ++i) {
    vec3 lightDir = normalize(-dirLight.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = worldAmbient;

    float dScalar = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * (dScalar * color);

    float sScalar = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = sScalar * material.specular;

    result += (ambient + diffuse + specular);
  }
  
  // Point Lights
  for (uint i=0; i<numPointLights; ++i) {
    vec3 lightDir = normalize(pointLights[i].position - modelPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float distance = length(pointLights[i].position - modelPos);
    float attenuation = 1.0f / (pointLights[i].constant + pointLights[i].linear*distance + pointLights[i].quadratic*(distance*distance));
    
    vec3 ambient = attenuation * worldAmbient;
    
    float dScalar = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = attenuation * (pointLights[i].diffuse * (dScalar * color));
    
    float sScalar = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = attenuation * (sScalar * material.specular);
    
    result += (ambient + diffuse + specular);
  }
  
  vColor = result;
}
