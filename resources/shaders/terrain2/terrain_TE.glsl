#version 430

layout (quads, fractional_odd_spacing, ccw) in;

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
        (1 - u) * (1 - v) * gl_in[15].gl_Position +
        u * (1 - v) * gl_in[3].gl_Position +
        u * v * gl_in[0].gl_Position +
        (1 - u) * v * gl_in[12].gl_Position
    );

    vec2 texCoord = (
        (1 - u) * (1 - v) * teTexCoord[15] +
        u * (1 - v) * teTexCoord[3] +
        u * v * teTexCoord[0] +
        (1 - u) * v * teTexCoord[12]
    );

    float height = texture(heightMap, texCoord).r * scaleY;

    position.y = height;
    //while (height == 0.0f) break;

    gTexCoord = texCoord;

    gl_Position = position;
}