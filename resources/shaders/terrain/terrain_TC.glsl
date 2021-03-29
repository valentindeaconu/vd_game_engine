#version 430

layout (vertices = 16) out;

in vec2 tcTexCoords[];
in vec4 tcTessFactor[];
in mat3 tcNormalMatrix[];

out vec2 teTexCoords[];
out mat3 teNormalMatrix[];

/*
               A        Outer[1] / AB      B
            (0, 0) -------------------- (1, 0)
               |                           |
               |                           |
               |                           |
 Outer[0] / DA |                           | Outer[2] / BC
               |                           |
               |                           |
               |                           |
            (0, 1) -------------------- (1, 1)
               D        Outer[3] / CD      C
*/

// patch edges indices
const int AB = 1;
const int BC = 2;
const int CD = 3;
const int DA = 0;

uniform float tessellationLevel;

uniform vec3 cameraPosition;

void main() {
    vec4 tessFactor = tcTessFactor[gl_InvocationID];

    // control only the first call
    if (gl_InvocationID == 0) {
        gl_TessLevelOuter[AB] = tessellationLevel * tessFactor[1];
        gl_TessLevelOuter[BC] = tessellationLevel * tessFactor[0];
        gl_TessLevelOuter[CD] = tessellationLevel * tessFactor[3];
        gl_TessLevelOuter[DA] = tessellationLevel * tessFactor[2];

        gl_TessLevelInner[0] = tessellationLevel / 2.0f;
        gl_TessLevelInner[1] = tessellationLevel / 2.0f;
    }

    teTexCoords[gl_InvocationID] = tcTexCoords[gl_InvocationID];
    teNormalMatrix[gl_InvocationID] = tcNormalMatrix[gl_InvocationID];

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}