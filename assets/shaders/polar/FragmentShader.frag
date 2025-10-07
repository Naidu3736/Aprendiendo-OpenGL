#version 460 core

in vec4 Color;                       // Color interpolado del vertex shader
out vec4 outColor;                   // Color final de salida

void main() {
    outColor = vec4(Color);     // Asignar color con alpha=1 (opaco)
}