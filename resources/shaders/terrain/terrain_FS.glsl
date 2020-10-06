#version 430

layout (location = 0) out vec4 fColor;

in vec2 fTexCoords;
in vec3 fPosition;
in vec3 fTangent;

uniform sampler2D normalMap;
uniform usampler2D splatMap;

#include "material_lib.glsl"

uniform int tbnRange;
uniform vec3 cameraPosition;

const vec3 direction = vec3(0.1f, -1.0f, 0.1f);
const float intensity = 1.2f;

float diffuse(vec3 direction, vec3 normal, float intensity) {
    return max(0.04f, dot(normal, -direction) * intensity);
}

void main() {
    vec3 normal = normalize(texture(normalMap, fTexCoords).rbg);

    float dist = length(cameraPosition - fPosition);
    float height = fPosition.y;

    uint splatMask = texture(splatMap, fTexCoords).r;

    vec4 materialColor = vec4(0.0f);
    float total = 0.0f;
    for (uint i = 0; i < MAX_MATERIALS; ++i) {
        uint msk = (1 << i);
        if ((splatMask & msk) > 0) {
            materialColor += texture(materials[i].diffuseMap, fTexCoords * materials[i].horizontalScaling);
            total = total + 1.0f;
        }
    }
    materialColor /= total;

    if (dist < tbnRange - 50) {
        float attenuation = clamp(-dist/(tbnRange - 50) + 1.0f, 0.0f, 1.0f);

        vec3 bitangent = normalize(cross(fTangent, normal));

        mat3 TBN = mat3(fTangent, normal, bitangent);

        vec3 bumpNormal = vec3(0.0f);
        for (uint i = 0; i < MAX_MATERIALS; ++i) {
            uint msk = (1 << i);
            if ((splatMask & msk) > 0) {
                vec3 materialNormal = texture(materials[i].normalMap, fTexCoords * materials[i].horizontalScaling).rbg;
                bumpNormal += (2.0f * materialNormal - 1.0f);
            }
        }
        bumpNormal = normalize(bumpNormal);
        bumpNormal.xz *= attenuation;
        normal = normalize(TBN * bumpNormal);
    }

    fColor = vec4(vec3(materialColor) * diffuse(direction, normal, intensity), materialColor.a);
}