#version 430

layout (quads, equal_spacing) in;

in vec2 teTexCoords[];
in mat3 teNormalMatrix[];

out vec2 gTexCoords;
out mat3 gNormalMatrix;

uniform sampler2D heightMap;
uniform float scaleY;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // using corners of the patch, we can compute the position
    vec4 position = (
        (1 - u) * (1 - v) * gl_in[15].gl_Position +
        u * (1 - v) * gl_in[3].gl_Position +
        u * v * gl_in[0].gl_Position +
        (1 - u) * v * gl_in[12].gl_Position
    );

    vec2 texCoords = (
        (1 - u) * (1 - v) * teTexCoords[15] +
        u * (1 - v) * teTexCoords[3] +
        u * v * teTexCoords[0] +
        (1 - u) * v * teTexCoords[12]
    );

    position.y = texture(heightMap, texCoords).r * scaleY;

    mat3 normalMatrix = (
        (1 - u) * (1 - v) * teNormalMatrix[15] +
        u * (1 - v) * teNormalMatrix[3] +
        u * v * teNormalMatrix[0] +
        (1 - u) * v * teNormalMatrix[12]
    );

    gTexCoords = texCoords;
    gNormalMatrix = normalMatrix;

    gl_Position = position;
}