#version 410 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vTexCoords;

out vec2 fTexCoords;

uniform vec4 transform;

void main() {
    vec2 position = vPosition * 0.5f;
    fTexCoords = vTexCoords; // position + 0.5f;

    vec2 screenCoords = position * transform.zw + transform.xy;
    
    screenCoords.x = screenCoords.x * 2.0f - 1.0f;
    screenCoords.y = screenCoords.y * -2.0f + 1.0f;

    gl_Position = vec4(screenCoords, 0.9999f, 1.0f);
}