#version 410 core

layout (location = 0) in vec4 vVertex; // <vec2 pos, vec2 tex>
out vec2 fTexCoords;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(vVertex.xy, 0.0, 1.0);
    fTexCoords = vVertex.zw;
}