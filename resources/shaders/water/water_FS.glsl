#version 410 core

in vec4 fPosition;
in vec3 fNormal;

in vec4 clipSpace;

in mat3 fNormalMatrix;
in mat3 fLightDirectionMatrix;
in float fVisibility;

out vec4 fColor;

uniform mat4 view;

uniform vec3 fogColor;

#include "../lib/light_FS.glsl"

uniform Light lights[MAX_LIGHTS];

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main() 
{
	vec2 normCoords = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;

	vec2 refractionTexCoords = vec2(normCoords.x, normCoords.y);
	vec2 reflectionTexCoords = vec2(normCoords.x, -normCoords.y);

	// get color from texture
	vec4 reflectionColor = texture(reflectionTexture, reflectionTexCoords);
	vec4 refractionColor = texture(refractionTexture, refractionTexCoords);

	vec4 waterColor = mix(reflectionColor, refractionColor, 0.5f);

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

	while (fogColor == vec3(1.0f)) break;
	while (fVisibility == 0.5f) break;

	// modulate with light
	vec4 lighting = vec4(min(totalMat.ambient + totalMat.diffuse + totalMat.specular, 1.0f), 1.0f);
	fColor = lighting;
	//fColor = mix(vec4(fogColor, 1.0f), lighting, fVisibility);
}
