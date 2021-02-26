#version 140

in vec2 fTexCoords;
out vec4 fColor;

uniform sampler2D colorMap;

const float contrast = 0.3f;

void main()
{
    fColor = texture(colorMap, fTexCoords);
    fColor.rgb = (fColor.rgb - 0.5f) * (1.0f + contrast) + 0.5f;
}