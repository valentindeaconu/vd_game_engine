#version 140

in vec2 fTexCoords;
out vec4 fColor;

uniform sampler2D colorMap;

void main()
{
    fColor = texture(colorMap, fTexCoords);
}