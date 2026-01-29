#version 460 core

layout (location = 0) in vec3 FragPos;    // En view space
layout (location = 1) in vec3 vNormal;    // En view space

layout (location = 0) out vec4 FragColor;

uniform vec3 uLightColor;
uniform vec3 uObjectColor;
uniform vec3 uLightPos;    // En world space
uniform vec3 uViewPos;     // En world space (posición de cámara)
uniform mat4 uView; 

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;
    
    // Convertir todo a view space
    vec3 lightPosView = vec3(uView * vec4(uLightPos, 1.0));
    vec3 viewPosView = vec3(uView * vec4(uViewPos, 1.0)); 
    
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPosView - FragPos);

    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    // Specular 
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPosView - FragPos); 
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * uLightColor;  

    vec3 result = (ambient + diffuse + specular) * uObjectColor;
    FragColor = vec4(result, 1.0);
}