#version 410 core

layout (location = 0) in vec3 vPosition;

out vec4 fPosition;
out vec2 fTexCoords;

out vec4 clipSpace;
out vec3 toCamera;
out vec3 fromLightVector;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// tiling
uniform float tiling;

// camera position for fesnel effect
uniform vec3 cameraPosition;

// lighting for normal mapping
uniform vec3 sunDirection;

void main() 
{
	// compute texcoords
	fTexCoords = vec2(vPosition.x / 2.0f + 0.5f, vPosition.z / 2.0f + 0.5f) * tiling;

	// compute world space coordinates
	vec4 worldCoordinates = model * vec4(vPosition, 1.0f);

	// compute eye space coordinates
	vec4 eyeSpaceCoordinates = view * worldCoordinates;
	fPosition = eyeSpaceCoordinates;

	// coompute to camera vector
	toCamera = cameraPosition - worldCoordinates.xyz;

	// compute from light vector
	fromLightVector = worldCoordinates.xyz - sunDirection;

	// compute vertex position
	clipSpace = projection * eyeSpaceCoordinates;
	gl_Position = clipSpace;
}
