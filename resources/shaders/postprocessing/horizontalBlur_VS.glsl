#version 140

in vec2 vPosition;

out vec2 fTexCoords[11];

uniform float targetWidth;

void main()
{
    gl_Position = vec4(vPosition, 0.0, 1.0);
    vec2 rootTexCoords = vPosition * 0.5f + 0.5f;
    float pixelSize = 1.0f / targetWidth;

    for (int i = -5; i <= 5; ++i) {
        fTexCoords[i + 5] = rootTexCoords + vec2(pixelSize * i, 0.0f);
    }
}