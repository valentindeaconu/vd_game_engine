#version 410 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec4 fPosition;
out vec3 fNormal;
out vec2 fTexCoords;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat3 uNormal;

uniform int uFakeLighting;
uniform vec4 uClipPlane;

void main()
{
    vec3 normal = vec3(0, 1, 0);
    if (uFakeLighting == 0) {
        normal = vNormal;
    }

    vec4 worldPosition = uModel * vec4(vPosition, 1.0f);
    vec4 eyePosition = uView * worldPosition;

    // Pass FS vars
    fPosition = eyePosition;
    fNormal = normalize(uNormal * normal); // compute normal in eye-space
    fTexCoords = vTexCoords;

    // Pass GL vars
    gl_Position = uProjection * eyePosition;
    gl_ClipDistance[0] = dot(worldPosition, uClipPlane);
}
