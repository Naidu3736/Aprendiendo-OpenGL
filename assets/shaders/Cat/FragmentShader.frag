#version 460 core

in vec4 Color;
in vec2 TexCoord;

uniform sampler2D Texture;

out vec4 FragColor;

void main() {
    vec4 texColor = texture(Texture, TexCoord);
    if (texColor.a < 0.1) 
        discard;
    FragColor = texColor;
}