#version 140

in vec2 fTexCoords;
out vec4 fColor;

uniform sampler2D depthMap;
uniform sampler2D colorMap;
uniform sampler2D blurMap;

uniform float near;
uniform float far;

const vec4 black = vec4(0.0f, 0.0f, 0.0f, 1.0f);
const vec4 white = vec4(1.0f);

float computeDistanceUsingDepth(float depth, float nearPlane, float farPlane) {
    return 2.0f * nearPlane * farPlane / (farPlane + nearPlane - (2.0f * depth - 1.0f) * (farPlane - nearPlane));
}

void main() {
    float depth = computeDistanceUsingDepth(texture(depthMap, fTexCoords).r, near, far) / 300.0f;
    depth = clamp(depth, 0.0f, 0.65f);

    vec4 color = texture(colorMap, fTexCoords);
    vec4 blur = texture(blurMap, fTexCoords);

    fColor = mix(color, blur, depth);
}