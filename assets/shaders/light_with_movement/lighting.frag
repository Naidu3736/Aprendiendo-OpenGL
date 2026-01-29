#version 460 core

layout (location = 0) in vec3 vFragPos;
layout (location = 1) in vec2 vTexCoords;
layout (location = 2) in vec3 vNormal;

layout (location = 0) out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec3 uLightColor;
uniform vec3 uLightPos;
uniform mat4 uView;
uniform float uAmbientStrength;
uniform float uSpecularStrength;
uniform float uBrightness;

void main()
{
    vec4 texColor = texture(uTexture, vTexCoords);

    vec3 ambient = uAmbientStrength * uLightColor;

    vec3 lightPosView = vec3(uView * vec4(uLightPos, 1.0));
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPosView - vFragPos);

    float diff = max(dot(norm, lightDir), 0.0); 
    vec3 diffuse = diff * uLightColor;

    vec3 viewDir = normalize(-vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), uBrightness);
    vec3 specular = uSpecularStrength * spec * uLightColor;

    vec3 result = (ambient + diffuse + specular) * texColor.rgb;
    FragColor = vec4(result, texColor.a);
}

