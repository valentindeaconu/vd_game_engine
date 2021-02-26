#version 150

in vec2 fTexCoords[11];
out vec4 fColor;

uniform sampler2D colorMap;

void main()
{
    fColor = vec4(0.0f);
    fColor += texture(colorMap, fTexCoords[0]) * 0.0093;
    fColor += texture(colorMap, fTexCoords[1]) * 0.028002;
    fColor += texture(colorMap, fTexCoords[2]) * 0.065984;
    fColor += texture(colorMap, fTexCoords[3]) * 0.121703;
    fColor += texture(colorMap, fTexCoords[4]) * 0.175713;
    fColor += texture(colorMap, fTexCoords[5]) * 0.198596;
    fColor += texture(colorMap, fTexCoords[6]) * 0.175713;
    fColor += texture(colorMap, fTexCoords[7]) * 0.121703;
    fColor += texture(colorMap, fTexCoords[8]) * 0.065984;
    fColor += texture(colorMap, fTexCoords[9]) * 0.028002;
    fColor += texture(colorMap, fTexCoords[10]) * 0.0093;
}
