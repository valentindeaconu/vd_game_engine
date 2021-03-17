#version 410 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vTexCoords;

out vec2 fTexCoords;

uniform mat4 transform;

void main()
{
    fTexCoords = vec2((vPosition.x + 1.0f) / 2.0f, 1.0f - (vPosition.y + 1.0f) / 2.0f);

    gl_Position = transform * vec4(vPosition, 0.0f, 1.0f);
}