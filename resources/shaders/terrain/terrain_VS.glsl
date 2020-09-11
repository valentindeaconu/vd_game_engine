#version 410 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoords;

out vec3 fNormal;
out vec4 fPosition;
out vec2 fTexCoords;

out mat3 fNormalMatrix;
out mat3 fLightDirectionMatrix;
out float fVisibility;

out vec4 fPosition_ls;

out float fShadowDistance;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightView;
uniform mat4 lightProjection;

// shadow constants
uniform float shadowDistance;
uniform float shadowTransitionDistance;

// fog constants
uniform float fogDensity;
uniform float fogGradient;

#include "../lib/fog_VS.glsl"

void main() 
{
	// pass normal and texcoords
	fNormal = vNormal;
	fTexCoords = vTexCoords;

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

	// compute light space coordinates
	fPosition_ls = lightProjection * lightView * worldCoordinates;

	// compute the distance for shadow transition
	float distance = length(cameraPosition.xyz);
	distance = distance - (shadowDistance - shadowTransitionDistance);
	distance = distance / shadowTransitionDistance;
	fShadowDistance = clamp(1.0f - distance, 0.0f, 1.0f);

	// compute vertex position
	gl_Position = projection * cameraPosition;
}
