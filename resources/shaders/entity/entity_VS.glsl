#version 410 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec4 fPosition;
out vec3 fNormal;
out vec2 fTexCoords;

out float fVisibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

uniform int fakeLighting = 0;

#include <fog.glsl>

uniform vec4 clipPlane;

void main() 
{
	// pass normal and texcoords
	if (fakeLighting == 0) {
		fNormal = normalize(normalMatrix * vNormal); // compute normal in eye-space
	} else {
		fNormal = normalize(normalMatrix * vec3(0, 1, 0));
	}

	fTexCoords = vTexCoords;

	// world coordinates
	vec4 worldPosition = model * vec4(vPosition, 1.0f);

	// clip plane
	gl_ClipDistance[0] = dot(worldPosition, clipPlane);

	// compute eye space coordinates
	vec4 cameraPosition = view * worldPosition;
	fPosition = cameraPosition;
	
	// compute vertex visibility
	fVisibility = GetVisibilityThruFog(cameraPosition.xyz, fog.Density, fog.Gradient);
	
	// compute vertex position
	gl_Position = projection * cameraPosition;
}
