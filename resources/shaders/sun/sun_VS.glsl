#version 410 core

layout (location = 0) in vec2 vPosition;

out vec2 gTexCoords;

uniform mat4 model;

void main() {
    gTexCoords = vPosition;

    gl_Position = model * vec4(vPosition.x, vPosition.y, 0.0f, 1.0f);
}