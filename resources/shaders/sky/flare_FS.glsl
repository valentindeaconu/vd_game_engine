#version 410 core

in vec2 fTexCoords;

out vec4 fColor;

uniform sampler2D diffuseMap;
uniform float brightness;

void main()
{
    fColor = texture(diffuseMap, fTexCoords);
    fColor.a *= brightness;
}