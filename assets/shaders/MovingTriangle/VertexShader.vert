#version 460 core

#pragma vscode_glsllint_stage : vert

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 Color;
uniform float xOffset;
uniform float yOffset;

void main() {
    gl_Position = vec4(aPos.x + xOffset, aPos.y + yOffset, aPos.z, 1.0f);
    Color = aColor;
}