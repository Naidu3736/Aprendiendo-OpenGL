#version 460 core

in vec3 Color;                       // Color interpolado del vertex shader
out vec4 outColor;                   // Color final de salida

void main() {
    outColor = vec4(Color, 1.0f);     // Asignar color con alpha=1 (opaco)
}