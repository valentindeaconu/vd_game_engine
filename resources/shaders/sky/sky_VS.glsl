#version 410 core
// #version 330

layout (location = 0) in vec3 vPosition;

out vec3 fPosition;
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
	vec4 worldPosition = model * vec4(vPosition, 1.0f);
	fPosition = worldPosition.xyz;
	
	vec4 cameraPosition = view * worldPosition;
	
	fVisibility = getObjectVisibilityThruFog(cameraPosition.xyz, fogDensity, fogGradient);
	
	gl_Position = projection * cameraPosition;
}



