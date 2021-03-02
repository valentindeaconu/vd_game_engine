#version 410 core

in vec2 fTexCoords;

out vec4 fColor;

uniform sampler2D diffuseMap;

void main() {
    vec4 diffuseColor = texture(diffuseMap, fTexCoords);

    if (diffuseColor.a < 0.01f) {
        discard;
    }

    fColor = diffuseColor;
}