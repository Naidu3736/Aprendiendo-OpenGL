#version 460 core

in vec4 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float uMix;

void main() {
    // Mezcla ambas texturas
    vec4 tex1 = texture(texture1, TexCoord);
    vec4 tex2 = texture(texture2, TexCoord);
    if (tex1.a < 0.1f) 
        discard;
    if (tex2.a < 0.1f) 
        discard;
    FragColor = mix(tex1, tex2, uMix) * Color;
}