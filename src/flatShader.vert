#version 330 core
struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};

struct DirLight {
  vec3 direction;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {
  vec3 position;
  
  float constant;
  float linear;
  float quadradic;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#define NR_POINT_LIGHTS 4

flat out vec3 vColor;

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0f);
  
  vec3 result;
  vec3 Normal = mat3(transpose(inverse(model))) * normal; // TODO: calculate Normal Matrix on CPU.
  vec3 modelPos = vec3(model * vec4(position, 1.0f));
  vec3 viewDir = normalize(viewPos - modelPos);
  
  // Directional Light
  {
    vec3 lightDir = normalize(-dirLight.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    vec3 ambient = dirLight.ambient * material.ambient;
    
    float dScalar = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = dirLight.diffuse * (dScalar * material.diffuse);
    
    float sScalar = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = dirLight.specular * (sScalar * material.specular);
    
    result = (ambient + diffuse + specular);
  }
  
  // Point Lights
  for (int i = 0; i < NR_POINT_LIGHTS; i++) {
    vec3 lightDir = normalize(pointLights[i].position - modelPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float distance = length(pointLights[i].position - modelPos);
    float attenuation = 1.0f / (pointLights[i].constant + pointLights[i].linear*distance + pointLights[i].quadradic*(distance*distance));
    
    vec3 ambient = attenuation * (pointLights[i].ambient * material.ambient);
    
    float dScalar = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = attenuation * (pointLights[i].diffuse * (dScalar * material.diffuse));
    
    float sScalar = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = attenuation * (pointLights[i].specular * (sScalar * material.specular));
    
    result += (ambient + diffuse + specular);
  }
  
  vColor = result;
}
