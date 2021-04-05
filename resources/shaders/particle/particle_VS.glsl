#version 410 core

layout (location = 0) in mat4 vModel;
layout (location = 4) in float vLifePercentage;

out mat4 gModel;
out float gLifePercentage;

void main() {
    gModel = vModel;
    gLifePercentage = vLifePercentage;
    
    gl_Position = vec4(0.5f, 0.5f, 0.0f, 1.0f);
}