#version 460 core

layout (location = 0) in vec3 vFragPos;
layout (location = 1) in vec2 vTexCoords;
layout (location = 2) in vec3 vNormal;

layout (location = 0) out vec4 FragColor;

uniform mat4 uView;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material uMaterial;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light uLight;

void main() 
{
    vec3 diffuseMap = vec3(texture(uMaterial.diffuse, vTexCoords));
    vec3 ambient = uLight.ambient * diffuseMap;

    vec3 lightPosView = vec3(uView * vec4(uLight.position, 1.0f));

    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPosView - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * diffuseMap) * uLight.diffuse;

    vec3 specularMap = vec3(texture(uMaterial.specular, vTexCoords));
    vec3 viewDir = normalize(-vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
    vec3 specular = (spec * specularMap) * uLight.specular;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);  
}

