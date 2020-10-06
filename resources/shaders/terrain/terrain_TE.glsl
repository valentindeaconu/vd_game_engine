#version 430

layout (quads, fractional_odd_spacing, cw) in;

in vec2 teTexCoords[];
out vec2 gTexCoords;

uniform float patchHeights[16];

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // using corners of the patch, we can compute the position
    // 0 = bottom left, 3 = bottom right, 12 = top left, 15 = top right
    vec4 v0 = vec4(gl_in[0].gl_Position.x, patchHeights[0], gl_in[0].gl_Position.z, 1.0f);
    vec4 v3 = vec4(gl_in[3].gl_Position.x, patchHeights[3], gl_in[3].gl_Position.z, 1.0f);
    vec4 v12 = vec4(gl_in[12].gl_Position.x, patchHeights[12], gl_in[12].gl_Position.z, 1.0f);
    vec4 v15 = vec4(gl_in[15].gl_Position.x, patchHeights[15], gl_in[15].gl_Position.z, 1.0f);

    vec4 position = (
        (1 - u) * (1 - v) * v12 +
        u * (1 - v) * v0 +
        u * v * v3 +
        (1 - u) * v * v15
    );

    vec2 texCoords = (
        (1 - u) * (1 - v) * teTexCoords[12] +
        u * (1 - v) * teTexCoords[0] +
        u * v * teTexCoords[3] +
        (1 - u) * v * teTexCoords[15]
    );

    gTexCoords = texCoords;
    gl_Position = position;
}