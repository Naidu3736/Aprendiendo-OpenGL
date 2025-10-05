#version 460 core

layout (location = 0) in vec2 aPos;  // Atributo de posición
layout (location = 1) in vec3 aColor; // Atributo de color
out vec3 Color;                      // Color de salida al fragment shader

void main() {
    gl_Position = vec4(aPos, 0.0f, 1.0f);   // Transformación a espacio de clip
    Color = aColor;                   // Pasar color al fragment shader
}
