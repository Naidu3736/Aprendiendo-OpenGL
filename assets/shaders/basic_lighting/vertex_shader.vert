#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

layout (location = 0) out vec3 FragPos; 
layout (location = 1) out vec3 vNormal;

uniform mat4 uProjection;
uniform mat4 uView; 
uniform mat4 uModel;

void main()
{
    mat4 modelView = uView * uModel;
    
    gl_Position = uProjection * modelView * vec4(aPos, 1.0);
    FragPos = vec3(modelView * vec4(aPos, 1.0));
    vNormal = mat3(transpose(inverse(modelView))) * aNormal;
}