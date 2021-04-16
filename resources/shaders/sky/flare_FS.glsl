#version 410 core

in vec2 fTexCoords;

out vec4 fColor;

uniform sampler2D diffuseMap;
uniform float brightness;

void main()
{
    vec4 color = texture(diffuseMap, fTexCoords);
    color.a *= brightness;

    fColor = color;

    //while (fColor == vec4(0)) break;

    //fColor = vec4(0, 1, 0.1, 1);
}