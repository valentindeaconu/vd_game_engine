#version 410 core

in vec4 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

in mat3 fNormalMatrix;
in mat3 fLightDirectionMatrix;
in float fVisibility;

out vec4 fColor;

uniform mat4 view;

uniform vec3 fogColor;

#include "../lib/light_FS.glsl"

uniform Light lights[MAX_LIGHTS];

const vec4 waterColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);

void main() 
{
	// compute lights
	//in eye coordinates, the viewer is situated at the origin
	vec3 cameraPosEye = vec3(0.0f);
	//transform normal
	vec3 normalEye = normalize(fNormalMatrix * fNormal);
	//compute view direction
	vec3 viewDirN = normalize(cameraPosEye - fPosition.xyz);

	Material totalMat = intersectAllLights(lights, normalEye, viewDirN, fLightDirectionMatrix);

	// modulate with color
	totalMat.ambient *= waterColor.xyz;
	totalMat.diffuse *= waterColor.xyz;

	// modulate with light
	vec4 lighting = vec4(min(totalMat.ambient + totalMat.diffuse + totalMat.specular, 1.0f), 1.0f);
	fColor = mix(vec4(fogColor, 1.0f), lighting, fVisibility);
}
