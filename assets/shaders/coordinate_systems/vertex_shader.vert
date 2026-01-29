#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoords;

layout (location = 0) out vec4 vColor;
layout (location = 1) out vec2 vTexCoords;

layout (location = 0) uniform mat4 uView;
layout (location = 1) uniform mat4 uProjection;
layout (location = 2) uniform mat4 uModel;

void main()
{
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);

    vColor = aColor;
    vTexCoords = aTexCoords;
}