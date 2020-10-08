#version 430

layout (location = 0) in vec2 vPosition;

out vec2 tcTexCoords;

uniform mat4 localModel;
uniform mat4 worldModel;

void main() {
    // Compute local coordinates
    vec2 localCoords = (localModel * vec4(vPosition.x, 0.0f, vPosition.y, 1.0f)).xz;

    // Pass texcoords
    tcTexCoords = localCoords;

    // Compute world coordinates
    vec4 worldCoords = worldModel * vec4(localCoords.x, 0.0f, localCoords.y, 1.0f);

    // Set vertex position
    gl_Position = worldCoords;
}