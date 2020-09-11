#version 410 core

in vec4 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

in mat3 fNormalMatrix;
in mat3 fLightDirectionMatrix;
in float fVisibility;

in vec4 fPosition_ls;

in float fShadowDistance;

out vec4 fColor;

uniform mat4 view;

uniform sampler2D shadowMap;
const int MAX_TEXTURES = 6;
uniform sampler2D textures[MAX_TEXTURES];
uniform usampler2D splatMap;

uniform vec3 fogColor;

#include "../lib/light_FS.glsl"

uniform Light lights[MAX_LIGHTS];

float computeShadow(vec4 fragPosLightSpace, vec3 lightDirection, float shadowDistance)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	if (projCoords.z > 1.0f)
		return 0.0f;

	// transform to [0,1] range
	projCoords = projCoords * 0.5f + 0.5f;

	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(shadowMap, projCoords.xy).r;

	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;

	//float bias = max(0.05f * (1.0f - dot(fNormal, lightDirection)), 0.005f);
	const float bias = 0.002f;

	// check whether current frag pos is in shadow
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;

			if (currentDepth - bias > pcfDepth)
			{
				shadow += (1.0f - shadowDistance * 0.4f);
			}
		}
	}
	shadow /= 9.0;

	return shadow;
}

void main() 
{
	// splatMap color
	uint splatMask = texture(splatMap, fTexCoords).r;
	
	vec2 nTexCoords = fTexCoords * 140.0f;
	
	vec4 textureColors[MAX_TEXTURES];
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		textureColors[i] = texture(textures[i], nTexCoords);
	}

	vec4 totalColor = vec4(0.0f);
	float total = 0.0f;
	for (uint i = 0; i < MAX_TEXTURES; ++i)
	{
		uint msk = (1 << i);
		if ((splatMask & msk) > 0)
		{
			totalColor += textureColors[i];
			total = total + 1.0f;
		}
	}
	
	totalColor /= total;
	
	// compute lights
	//in eye coordinates, the viewer is situated at the origin
	vec3 cameraPosEye = vec3(0.0f);
	//transform normal
	vec3 normalEye = normalize(fNormalMatrix * fNormal);	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosition.xyz);
	
	Material totalMat = intersectAllLights(lights, normalEye, viewDirN, fLightDirectionMatrix);

	// modulate with material
	totalMat.ambient *= totalColor.xyz;
	totalMat.diffuse *= totalColor.xyz;

	// modulate with shadow
	vec3 lightDirection = -lights[0].position;
	float shadow = computeShadow(fPosition_ls, lightDirection, fShadowDistance);

	// modulate with light
	vec4 lighting = vec4(min(totalMat.ambient + (1.0f - shadow) * (totalMat.diffuse + totalMat.specular), 1.0f), 1.0f);
	fColor = mix(vec4(fogColor, 1.0f), lighting, fVisibility);
}
