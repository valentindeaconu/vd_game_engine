#version 430

layout (quads, fractional_odd_spacing, cw) in;

in vec2 teTexCoord[];
out vec2 gTexCoord;

uniform sampler2D heightMap;
uniform float scaleY;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // using corners of the patch, we can compute the position
    // 0 = bottom left, 3 = bottom right, 12 = top left, 15 = top right
    vec4 position = (
        (1 - u) * (1 - v) * gl_in[12].gl_Position +
        u * (1 - v) * gl_in[0].gl_Position +
        u * v * gl_in[3].gl_Position +
        (1 - u) * v * gl_in[15].gl_Position
    );

    vec2 texCoord = (
        (1 - u) * (1 - v) * teTexCoord[12] +
        u * (1 - v) * teTexCoord[0] +
        u * v * teTexCoord[3] +
        (1 - u) * v * teTexCoord[15]
    );

    position.y = texture(heightMap, texCoord).r * scaleY;

    gTexCoord = texCoord;

    gl_Position = position;
}