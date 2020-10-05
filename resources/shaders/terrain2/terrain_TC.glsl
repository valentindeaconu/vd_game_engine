#version 430

layout (vertices = 16) out;

in vec2 tcTexCoord[];
out vec2 teTexCoord[];

// patch edges indices
const int AB = 2;
const int BC = 3;
const int CD = 0;
const int DA = 1;

uniform int tessellationFactor;
uniform float tessellationSlope;
uniform float tessellationShift;

uniform vec3 cameraPosition;

float LodFactor(float dist) {
    return max(0.0f, tessellationFactor / pow(dist, tessellationSlope) + tessellationShift);
}

void main() {
    // control only the first call
    if (gl_InvocationID == 0) {
        vec3 abMid = vec3(gl_in[0].gl_Position + gl_in[3].gl_Position) / 2.0f;
        vec3 bcMid = vec3(gl_in[3].gl_Position + gl_in[15].gl_Position) / 2.0f;
        vec3 cdMid = vec3(gl_in[15].gl_Position + gl_in[12].gl_Position) / 2.0f;
        vec3 daMid = vec3(gl_in[12].gl_Position + gl_in[0].gl_Position) / 2.0f;

        float distanceAB = distance(abMid, cameraPosition);
        float distanceBC = distance(bcMid, cameraPosition);
        float distanceCD = distance(cdMid, cameraPosition);
        float distanceDA = distance(daMid, cameraPosition);

        while (LodFactor(distanceAB) == 1.0f) break;
        while (LodFactor(distanceBC) == 1.0f) break;
        while (LodFactor(distanceCD) == 1.0f) break;
        while (LodFactor(distanceDA) == 1.0f) break;

        gl_TessLevelOuter[AB] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceAB));
        gl_TessLevelOuter[BC] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceBC));
        gl_TessLevelOuter[CD] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceCD));
        gl_TessLevelOuter[DA] = mix(1, gl_MaxTessGenLevel, LodFactor(distanceDA));

        gl_TessLevelInner[0] = (gl_TessLevelOuter[BC] + gl_TessLevelOuter[DA]) / 4.0f;
        gl_TessLevelInner[1] = (gl_TessLevelOuter[AB] + gl_TessLevelOuter[CD]) / 4.0f;
    }

    teTexCoord[gl_InvocationID] = tcTexCoord[gl_InvocationID];

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}