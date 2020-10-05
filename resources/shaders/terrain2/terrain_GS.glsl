#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 gTexCoord[];

out vec2 fTexCoord;
out vec3 fPosition;
out vec3 fTangent;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;

uniform int tbnRange;

#include "material_lib.glsl"

vec3 tangent;
vec3 displacement[3];

void computeTangent() {
    vec3 v0 = gl_in[0].gl_Position.xyz;
    vec3 v1 = gl_in[1].gl_Position.xyz;
    vec3 v2 = gl_in[2].gl_Position.xyz;

    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;

    vec2 uv0 = gTexCoord[0];
    vec2 uv1 = gTexCoord[1];
    vec2 uv2 = gTexCoord[2];

    vec2 deltaUV1 = uv1 - uv0;
    vec2 deltaUV2 = uv2 - uv0;

    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

    tangent = normalize((e1 * deltaUV2.y - e2 * deltaUV1.y) * r);
}

void main() {
    float dist = (
        distance(gl_in[0].gl_Position.xyz, cameraPosition) +
        distance(gl_in[1].gl_Position.xyz, cameraPosition) +
        distance(gl_in[2].gl_Position.xyz, cameraPosition)
    ) / 3.0f;

    if (dist < tbnRange) {
        computeTangent();
    }

    for (int i = 0; i < gl_in.length(); ++i) {
        vec4 worldCoords = gl_in[i].gl_Position;
        vec4 eyeSpaceCoords = view * worldCoords;
        gl_Position = projection * eyeSpaceCoords;

        if (i == 0) {
            fTexCoord = gTexCoord[0];
        }
        else if (i == 1) {
            fTexCoord = gTexCoord[1];
        }
        else {
            fTexCoord = gTexCoord[2];
        }

        fPosition = eyeSpaceCoords.xyz;
        fTangent = tangent;
        EmitVertex();
    }

    EndPrimitive();
}