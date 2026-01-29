#version 460 core

layout (location = 0) in vec4 vLightColor;
layout (location = 1) in vec2 vTexCoords;

layout (location = 0) out vec4 FragColor;

uniform sampler2D uTexture;

void main()
{
    vec4 texColor = texture(uTexture, vTexCoords);
    FragColor = vLightColor * texColor;
}

