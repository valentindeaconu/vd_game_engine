#version 430

layout (location = 0) out vec4 fColor;

in vec2 fTexCoords;
in vec3 fPosition;
in vec3 fTangent;
in vec4 fPosition_ls;
in mat3 fNormalMatrix;
in float fRadiusClip;

// terrain material color
uniform sampler2D shadowMap;
uniform sampler2D normalMap;
uniform sampler2D splatMap;
#include "material_lib.glsl"

uniform int highDetailRange;
uniform vec3 cameraPosition;
uniform mat4 view;

// light constants
#include <light.glsl>

// shadow constants
uniform float shadowDistance;
uniform float shadowTransitionDistance;

#include <fog.glsl>

// wireframe
uniform int isWireframe;
uniform vec4 wireframeColor;

float computeShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDirection, float shadowDistance) {
    const int pcfCount = 2;
    const float totalTexels = (pcfCount * 2.0f + 1.0f) * (pcfCount * 2.0f + 1.0f);

    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    if (projCoords.z > 1.0f)
        return 0.0f;

    // transform to [0,1] range
    projCoords = projCoords * 0.5f + 0.5f;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    float bias = max(0.0075f * (1.0f - dot(normal, lightDirection)), 0.005f);

    // check whether current frag pos is in shadow
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -pcfCount; x <= pcfCount; ++x) {
        for (int y = -pcfCount; y <= pcfCount; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;

            if (currentDepth - bias > pcfDepth) {
                shadow += 1.0f;
            }
        }
    }
    shadow /= totalTexels;

    return shadow * shadowDistance;
}

void main() {
    if (isWireframe == 1) {
        fColor = wireframeColor;
    } else {
        vec3 normal = normalize(2.0f * texture(normalMap, fTexCoords).rbg - 1.0f);

        // compute material color
        float[16] blendSampleArray = BlendSample(splatMap, fTexCoords);

        vec4 materialColor = vec4(0.0f);
        for (int i = 0; i < MAX_MATERIALS; ++i) {
            materialColor += texture(materials[i].diffuseMap, fTexCoords * materials[i].horizontalScaling) * blendSampleArray[i];
        }

        // compute normal
        vec3 normalEye = normal;

        float dist = length(cameraPosition - fPosition);
        if (dist < highDetailRange - 50) {
            float attenuation = clamp(-dist/(highDetailRange - 50) + 1.0f, 0.0f, 1.0f);

            vec3 bitangent = normalize(cross(fTangent, normal));

            mat3 TBN = mat3(fTangent, bitangent, normal);

            vec3 bumpNormal = vec3(0.0f);
            for (int i = 0; i < MAX_MATERIALS; ++i) {
                vec3 materialNormal = texture(materials[i].normalMap, fTexCoords * materials[i].horizontalScaling).rbg * blendSampleArray[i];
                bumpNormal += (2.0f * materialNormal - 1.0f);
            }

            bumpNormal = normalize(bumpNormal);
            bumpNormal.xz *= attenuation;
            normal = normalize(TBN * bumpNormal);

            // from tangent space to world space
            normalEye = normalize(transpose(TBN) * normal);
        }

        // compute lights
        // in eye coordinates, the viewer is situated at the origin
        vec3 cameraPosEye = vec3(0.0f);
        // from world space to light space
        normalEye = normalize(fNormalMatrix * normalEye);
        // compute view direction
        vec3 viewDirN = normalize(cameraPosition - fPosition);
        // compute light direction matrix
        mat3 lightDirectionMatrix = mat3(transpose(inverse(view)));

        // compute shadow
        float distance = (length(fPosition) - (shadowDistance - shadowTransitionDistance)) / shadowTransitionDistance;
        float shadowDistanceFactor = clamp(1.0f - distance, 0.0f, 1.0f);
        float shadow = computeShadow(fPosition_ls, normal, sun.Direction, shadowDistanceFactor);

        // modulate with lights
        Material material;
        material.Ambient = materialColor.xyz;
        material.Diffuse = materialColor.xyz;

        vec3 lighting = modulateWithLightsAndShadow(sun, lights, normalEye, viewDirN, lightDirectionMatrix, fPosition.xyz, material, shadow);

        // compute visibility factor
        float visibility = GetVisibilityThruFog(fPosition, fog.Density, fog.Gradient);

        // modulate with fog (based on distance from camera)
        vec4 fogColor = vec4(fog.Color, 1.0f);
        vec4 withFog = mix(fogColor, vec4(lighting, 1.0f), visibility);

        // modulate with fog (based on circular shaped terrain)
        fColor = mix(withFog, fogColor, fRadiusClip);
    }
}