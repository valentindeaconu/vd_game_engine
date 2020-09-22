#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

in vec2 gTexCoord[];

out vec2 fTexCoord;
out vec3 fPosition;
out vec3 fTangent;

uniform mat4 view;
uniform mat4 projection;
uniform sampler2D normalMap;
uniform usampler2D splatMap;
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
    for (int i = 0; i < 3; ++i) {
        displacement[i] = vec3(0.0f);
    }

    float dist = (
        distance(gl_in[0].gl_Position.xyz, cameraPosition) +
        distance(gl_in[1].gl_Position.xyz, cameraPosition) +
        distance(gl_in[2].gl_Position.xyz, cameraPosition)
    ) / 3.0f;

    if (dist < tbnRange) {
        computeTangent();

        for (int k = 0; k < gl_in.length(); ++k) {
            displacement[k] = vec3(0.0f, 1.0f, 0.0f);

            float height = gl_in[k].gl_Position.y;

            vec3 normal = normalize(texture(normalMap, gTexCoord[k]).rbg);

            uint splatMask = texture(splatMap, fTexCoord).r;

            float scale = 0.0f;
            for (uint i = 0; i < MAX_MATERIALS; ++i) {
                uint msk = (1 << i);
                if ((splatMask & msk) > 0) {
                    vec2 scaledTexCoords = gTexCoord[k] * materials[i].horizontalScaling;

                    float total = texture(materials[i].displaceMap, scaledTexCoords).r;
                    total *= materials[i].heightScaling;

                    scale += total;
                }
            }

            float d = distance(gl_in[k].gl_Position.xyz, cameraPosition);
            float attenuation = clamp(-d / (tbnRange - 50) + 1.0f, 0.0f, 1.0f);
            scale *= attenuation;

            displacement[k] *= scale;
        }
    }

    for (int i = 0; i < gl_in.length(); ++i) {
        vec4 worldPosition = gl_in[i].gl_Position + vec4(displacement[i], 0.0f);
        gl_Position = projection * view * worldPosition;
        fTexCoord = gTexCoord[i];
        fPosition = worldPosition.xyz;
        fTangent = tangent;
        EmitVertex();
    }

    EndPrimitive();
}