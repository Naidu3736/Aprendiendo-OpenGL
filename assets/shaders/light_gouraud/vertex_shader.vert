#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;


layout (location = 0) out vec4 vLightColor;
layout (location = 1) out vec2 vTexCoords;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uModel;
uniform vec3 uLightColor;
uniform vec3 uLightPos;
uniform float uAmbientStrength;
uniform float uSpecularStrength;
uniform float uBrightness;

void main()
{
    mat4 modelView = uView * uModel;

    gl_Position = uProjection * modelView * vec4(aPos, 1.0);
    vec3 fragPos = vec3(modelView * vec4(aPos, 1.0));

    vec3 norm = normalize(mat3(transpose(inverse(modelView))) * aNormal);

    vec3 ambient = uAmbientStrength * uLightColor;

    vec3 lightPosView = vec3(uView * vec4(uLightPos, 1.0));
    vec3 lightDir = normalize(lightPosView - fragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uBrightness);
    vec3 specular = uSpecularStrength * spec * uLightColor;

    vLightColor = vec4(ambient + diffuse + specular, 1.0);                                                     
    vTexCoords = aTexCoords;
}