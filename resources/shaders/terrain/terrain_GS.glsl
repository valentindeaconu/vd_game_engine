#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec2 gTexCoords[];

out vec2 fTexCoords;
out vec3 fPosition;
out vec3 fTangent;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;

uniform int tbnRange;

vec3 computeTangent(vec3 v0, vec3 v1, vec3 v2, vec2 uv0, vec2 uv1, vec2 uv2) {
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;

    vec2 deltaUV1 = uv1 - uv0;
    vec2 deltaUV2 = uv2 - uv0;

    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

    return normalize((e1 * deltaUV2.y - e2 * deltaUV1.y) * r);
}

void main() {
    vec3 tangent = vec3(0.0f);

    float dist = (
        distance(gl_in[0].gl_Position.xyz, cameraPosition) +
        distance(gl_in[1].gl_Position.xyz, cameraPosition) +
        distance(gl_in[2].gl_Position.xyz, cameraPosition)
    ) / 3.0f;

    if (dist < tbnRange) {
        tangent = computeTangent(gl_in[0].gl_Position.xyz, gl_in[1].gl_Position.xyz, gl_in[2].gl_Position.xyz, gTexCoords[0], gTexCoords[1], gTexCoords[2]);
    }

    for (int i = 0; i < gl_in.length(); ++i) {
        vec4 worldCoords = gl_in[i].gl_Position;
        vec4 eyeSpaceCoords = view * worldCoords;
        gl_Position = projection * eyeSpaceCoords;

        if (i == 0) {
            fTexCoords = gTexCoords[0];
        }
        else if (i == 1) {
            fTexCoords = gTexCoords[1];
        }
        else if (i == 2) {
            fTexCoords = gTexCoords[2];
        } else {
            fTexCoords = vec2(1.0f, 1.0f);
        }

        fPosition = eyeSpaceCoords.xyz;
        fTangent = tangent;

        EmitVertex();
    }

    EndPrimitive();
}