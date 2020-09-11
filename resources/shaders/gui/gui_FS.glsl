#version 410 core

in vec2 fTexCoords;

out vec4 fColor;

uniform sampler2D guiTexture;

void main()
{
    fColor = texture(guiTexture, fTexCoords);
}