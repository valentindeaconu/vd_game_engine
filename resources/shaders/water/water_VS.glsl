#version 410 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;

out vec3 fNormal;
out vec4 fPosition;

out vec4 clipSpace;

out mat3 fNormalMatrix;
out mat3 fLightDirectionMatrix;
out float fVisibility;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// fog constants
uniform float fogDensity;
uniform float fogGradient;

#include "../lib/fog_VS.glsl"

void main() 
{
	// pass normal and texcoords
	fNormal = vNormal;

	// compute world coordinates
	vec4 worldCoordinates = model * vec4(vPosition, 1.0f);

	// compute eye space coordinates
	vec4 cameraPosition = view * worldCoordinates;
	fPosition = cameraPosition;
	
	// compute normal matrix
	fNormalMatrix = mat3(transpose(inverse(view * model)));
	
	// compute light direction matrix
	fLightDirectionMatrix = mat3(transpose(inverse(view)));
	
	// compute vertex visibility
	fVisibility = getObjectVisibilityThruFog(cameraPosition.xyz, fogDensity, fogGradient);

	// compute vertex position
	clipSpace = projection * cameraPosition;
	gl_Position = clipSpace;
}
