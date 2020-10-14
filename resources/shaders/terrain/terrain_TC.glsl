#version 430

layout (vertices = 16) out;

in vec2 tcTexCoords[];
out vec2 teTexCoords[];

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

uniform int tessellationFactor;
uniform float tessellationSlope;
uniform float tessellationShift;

uniform float tessellationOuterLevel;
uniform float tessellationInnerLevel;

uniform vec4 tessFactor;
uniform vec3 edgeMid[4];

uniform vec3 cameraPosition;

float LodFactor(float dist) {
    return max(0.0f, tessellationFactor / pow(dist, tessellationSlope) + tessellationShift);
}

void main() {
    // control only the first call
    if (gl_InvocationID == 0) {
        float distanceAB = distance(edgeMid[AB], cameraPosition);
        float distanceBC = distance(edgeMid[BC], cameraPosition);
        float distanceCD = distance(edgeMid[CD], cameraPosition);
        float distanceDA = distance(edgeMid[DA], cameraPosition);

        float tessLevel_AB = mix(1, gl_MaxTessGenLevel, LodFactor(distanceAB));
        float tessLevel_BC = mix(1, gl_MaxTessGenLevel, LodFactor(distanceBC));
        float tessLevel_CD = mix(1, gl_MaxTessGenLevel, LodFactor(distanceCD));
        float tessLevel_DA = mix(1, gl_MaxTessGenLevel, LodFactor(distanceDA));

        while (tessLevel_AB == 0.0f) break;
        while (tessLevel_BC == 0.0f) break;
        while (tessLevel_CD == 0.0f) break;
        while (tessLevel_DA == 0.0f) break;

        tessLevel_AB = tessellationOuterLevel;
        tessLevel_BC = tessellationOuterLevel;
        tessLevel_CD = tessellationOuterLevel;
        tessLevel_DA = tessellationOuterLevel;

        gl_TessLevelOuter[AB] = tessLevel_AB * tessFactor[1]; // 0, 1, 3,
        gl_TessLevelOuter[BC] = tessLevel_BC * tessFactor[0]; // 1, 2, 0,
        gl_TessLevelOuter[CD] = tessLevel_CD * tessFactor[3]; // 2, 3, 1,
        gl_TessLevelOuter[DA] = tessLevel_DA * tessFactor[2]; // 3, 0, 2

        gl_TessLevelInner[0] = tessellationInnerLevel; // (tessLevel_BC + tessLevel_DA) / 4.0f;
        gl_TessLevelInner[1] = tessellationInnerLevel; // (tessLevel_AB + tessLevel_CD) / 4.0f;
    }

    teTexCoords[gl_InvocationID] = tcTexCoords[gl_InvocationID];
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}