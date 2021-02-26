#version 140

in vec2 vPosition;
out vec2 fTexCoords;

void main()
{
    gl_Position = vec4(vPosition, 0.0, 1.0);
    fTexCoords = vPosition * 0.5f + 0.5f;
}