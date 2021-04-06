#version 410 core

layout (location = 0) in mat4 vModelView;
layout (location = 4) in float vLifePercentage;

out mat4 gModelView;
out float gLifePercentage;

void main() {
    gModelView = vModelView;
    gLifePercentage = vLifePercentage;
    
    gl_Position = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}