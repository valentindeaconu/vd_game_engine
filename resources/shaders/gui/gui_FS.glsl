#version 410 core

in vec2 fTexCoords;

out vec4 fColor;

uniform int transparency = 0;

uniform sampler2D diffuseMap;

void main()
{
    vec4 color = texture(diffuseMap, fTexCoords);

    if (transparency == 1 && color.a < 0.1f) {
        discard;
    }

    fColor = color;
}