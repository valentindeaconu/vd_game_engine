#version 410 core

in vec2 fTexCoords;
out vec4 fColor;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, fTexCoords).r);
    // while (textColor == vec3(1.0f)) break;
    fColor = vec4(textColor, 1.0) * sampled;
    // while ( vec4(textColor, 1.0) * sampled == vec4(1.0f) ) break;
    // fColor = vec4(1.0f);
}