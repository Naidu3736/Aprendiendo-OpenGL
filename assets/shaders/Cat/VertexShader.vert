#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform float offsetTexCoord;

uniform mat4 model;

out vec4 Color;
out vec2 TexCoord;

void main() {
    gl_Position = model * vec4(aPos, 1.0f);
    Color = aColor;
    TexCoord = vec2(aTexCoord.x + offsetTexCoord, aTexCoord.y);
}