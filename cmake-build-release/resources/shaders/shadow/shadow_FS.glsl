#version 410 core

in vec2 fTexCoords;

out vec4 fColor;

uniform sampler2D diffuseMap;

void main()
{
    float alphaValue = texture(diffuseMap, fTexCoords).a;

    if (alphaValue < 0.01f) {
        discard;
    }

    fColor = vec4(1.0f);
}