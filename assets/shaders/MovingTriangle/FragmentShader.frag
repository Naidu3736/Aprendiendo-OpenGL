#version 460 core

#pragma vscode_glsllint_stage : frag

in vec4 Color;
out vec4 fragColor;

void main() {
    fragColor = Color;
}