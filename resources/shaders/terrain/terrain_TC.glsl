#version 430

layout (vertices = 16) out;

in vec2 tcTexCoords[];
out vec2 teTexCoords[];

uniform float tessellationOuterLevel;
uniform float tessellationInnerLevel;

/*
               D        Outer[3] / CD      C
            (0, 1) -------------------- (1, 1)
               |                           |
               |                           |
               |                           |
 Outer[0] / DA |                           | Outer[2] / BC
               |                           |
               |                           |
               |                           |
            (0, 0) -------------------- (1, 0)
               A        Outer[1] / AB      B
*/

// patch edges indices
const int AB = 1;
const int BC = 2;
const int CD = 3;
const int DA = 0;

void main() {
    // control only the first call
    if (gl_InvocationID == 0) {
        gl_TessLevelOuter[AB] = tessellationOuterLevel;
        gl_TessLevelOuter[BC] = tessellationOuterLevel;
        gl_TessLevelOuter[CD] = tessellationOuterLevel;
        gl_TessLevelOuter[DA] = tessellationOuterLevel;

        gl_TessLevelInner[0] = tessellationInnerLevel;
        gl_TessLevelInner[1] = tessellationInnerLevel;
    }

    teTexCoords[gl_InvocationID] = tcTexCoords[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}