#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 gTexCoords[];

out vec2 fTexCoords;
out vec3 fPosition;
out vec3 fTangent;
out vec4 fPosition_ls;

out mat3 fNormalMatrix;

out float fRadiusClip;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 localModel;
uniform mat4 worldModel;

uniform mat4 lightView;
uniform mat4 lightProjection;

uniform vec3 cameraPosition;

uniform int highDetailRange;

uniform sampler2D splatMap;

#include "material_lib.glsl"

uniform vec4 clipPlane;

uniform vec2 radius;
uniform vec2 center;

vec2 getTexCoords(int i) {
    if (i == 0) {
        return gTexCoords[0];
    }
    else if (i == 1) {
        return gTexCoords[1];
    }
    else {
        return gTexCoords[2];
    }
}

vec3 computeTangent(vec3 v0, vec3 v1, vec3 v2, vec2 uv0, vec2 uv1, vec2 uv2) {
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;

    vec2 deltaUV1 = uv1 - uv0;
    vec2 deltaUV2 = uv2 - uv0;

    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

    return normalize((e1 * deltaUV2.y - e2 * deltaUV1.y) * r);
}

float computeRadiusClip(vec4 worldCoords) {
    vec4 zeroWC = vec4(worldCoords.x, .0f, worldCoords.z, worldCoords.w);
    vec4 center4 = vec4(center.x, .0f, center.y, .0f);

    float dist = length(zeroWC - center4);

    if (dist > radius.x) {
        float factor = (dist - radius.x) / (radius.y - radius.x);
        factor = clamp(factor, 0.0f, 1.0f);
        return factor;
    } else {
        return .0f;
    }
}

void main() {
    vec3 tangent = vec3(0.0f);

    float dist = (
        distance(gl_in[0].gl_Position.xyz, cameraPosition) +
        distance(gl_in[1].gl_Position.xyz, cameraPosition) +
        distance(gl_in[2].gl_Position.xyz, cameraPosition)
    ) / 3.0f;

    float displacement[3] = { 0.0f, 0.0f, 0.0f };
    if (dist < highDetailRange) {
        tangent = computeTangent(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gTexCoords[0], gTexCoords[1], gTexCoords[2]);

        for (int k = 0; k < gl_in.length(); ++k) {
            float height = gl_in[k].gl_Position.y;

            // vec4 blendSample = texture(splatMap, getTexCoords(k));
            // float[4] blendSampleArray = float[](blendSample.r, blendSample.g, blendSample.b, blendSample.a);
            float[16] blendSampleArray = BlendSample(splatMap, getTexCoords(k));

            float scale = 0.0f;
            for (int i = 0; i < MAX_MATERIALS; ++i) {
                scale += texture(materials[i].displaceMap, getTexCoords(k) * materials[i].horizontalScaling).r 
                        * materials[i].heightScaling
                        * blendSampleArray[i];
            }
            
            // attenuate the scale factor using the distance to the vertex
            scale *= clamp(-distance(gl_in[k].gl_Position.xyz, cameraPosition) / (highDetailRange - 50) + 1.0f, 0.0f, 1.0f);

            displacement[k] = scale;
        }
    }

    for (int i = 0; i < gl_in.length(); ++i) {
        vec4 worldCoords = gl_in[i].gl_Position + vec4(0.0f, displacement[i], 0.0f, 0.0f);

        vec4 eyeSpaceCoords = view * worldCoords;
        gl_Position = projection * eyeSpaceCoords;

        // Set clipping plane
        gl_ClipDistance[0] = dot(worldCoords, clipPlane);

        fTexCoords = getTexCoords(i);

        fPosition = eyeSpaceCoords.xyz;
        fTangent = tangent;
        fPosition_ls = lightProjection * lightView * worldCoords;

        fNormalMatrix = transpose(inverse(mat3(view * (worldModel + localModel))));

        fRadiusClip = computeRadiusClip(worldCoords);

        EmitVertex();
    }

    EndPrimitive();
}