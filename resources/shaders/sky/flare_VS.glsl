#version 410 core

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vTexCoords;

out vec2 fTexCoords;

uniform vec4 transform;

void main() {
    vec2 position = vPosition * 0.5f;
    fTexCoords = position + 0.5f;

    vec2 screenCoords = position * transform.zw + transform.xy;

    // fTexCoords = vec2((vPosition.x + 1.0f) * 0.5f, 1.0f - (vPosition.y + 1.0f) * 0.5f);

    // vec2 sPosition = (vPosition * vec2(0.5f, 0.5f)) * transform.zw + transform.xy;

    // convert to opengl coord system 
    // sPosition.x = sPosition.x * 2.0f - 1.0f;
    // sPosition.y = sPosition.y * -2.0f + 1.0f;
    
    screenCoords.x = screenCoords.x * 2.0f - 1.0f;
    screenCoords.y = screenCoords.y * -2.0f + 1.0f;

    gl_Position = vec4(screenCoords, 0.0f, 1.0f);
}