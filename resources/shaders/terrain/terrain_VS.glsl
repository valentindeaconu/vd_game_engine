#version 430

layout (location = 0) in vec2 vPosition;
layout (location = 1) in mat4 vLocalModel;
layout (location = 5) in vec4 vTessFactor;

// uniform mat4 vLocalModel;
// uniform vec4 vTessFactor;

out vec2 tcTexCoords;
out vec4 tcTessFactor;
out mat3 tcNormalMatrix;

uniform mat4 worldModel;
uniform mat4 view;

uniform sampler2D heightMap;

void main() {
    // Compute local coordinates
    vec2 localCoords = (vLocalModel * vec4(vPosition.x, 0.0f, vPosition.y, 1.0f)).xz;

    // Pass texcoords and tessFactor
    tcTexCoords = localCoords;
    tcTessFactor = vTessFactor;

    // Compute normal matrix (transpose inverse of model-view matrix, without translation)
    tcNormalMatrix = transpose(inverse(mat3(view * (worldModel * vLocalModel))));

    // Read vertex' height
    float height = texture(heightMap, localCoords).r;

    // Compute world coordinates
    vec4 worldCoords = worldModel * vec4(localCoords.x, 0.0f, localCoords.y, 1.0f);

    // Set vertex position
    gl_Position = worldCoords;
}