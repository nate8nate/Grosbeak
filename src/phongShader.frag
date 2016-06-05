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

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutOff;
  float outerCutOff;
  
  float constant;
  float linear;
  float quadradic;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#define NR_POINT_LIGHTS 4

out vec4 fColor;

in vec3 vNormal;
in vec3 vModelPos;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

void main() {
  vec3 result;
  vec3 normal = normalize(vNormal);
  vec3 viewDir = normalize(viewPos - vModelPos);
  
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
    vec3 lightDir = normalize(pointLights[i].position - vModelPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float distance = length(pointLights[i].position - vModelPos);
    float attenuation = 1.0f / (pointLights[i].constant + pointLights[i].linear*distance + pointLights[i].quadradic*(distance*distance));
    
    vec3 ambient = attenuation * (pointLights[i].ambient * material.ambient);
    
    float dScalar = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = attenuation * (pointLights[i].diffuse * (dScalar * material.diffuse));
    
    float sScalar = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = attenuation * (pointLights[i].specular * (sScalar * material.specular));
    
    result += (ambient + diffuse + specular);
  }
  
//  // Spot Light
//  {
//    vec3 lightDir = normalize(spotLight.position - vModelPos);
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float distance = length(spotLight.position - vModelPos);
//    float attenuation = 1.0f / (spotLight.constant + spotLight.linear*distance + spotLight.quadradic*(distance*distance));
//    
//    float theta = dot(lightDir, normalize(-spotLight.direction));
//    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
//    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
//    
//    vec3 ambient = attenuation * intensity * (spotLight.ambient * material.ambient);
//    
//    float dScalar = max(dot(normal, lightDir), 0.0);
//    vec3 diffuse = attenuation * intensity * (spotLight.diffuse * (dScalar * material.diffuse));
//    
//    float sScalar = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    vec3 specular = attenuation * intensity * (spotLight.specular * (sScalar * material.specular));
//    
//    result += (ambient + diffuse + specular);
//  }
  
  
  fColor = vec4(result, 1.0f);
}
