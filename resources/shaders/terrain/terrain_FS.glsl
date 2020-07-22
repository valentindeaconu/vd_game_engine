#version 410 core

in vec4 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

in mat3 fNormalMatrix;
in mat3 fLightDirectionMatrix;
in float fVisibility;

out vec4 fColor;

uniform mat4 view;

const int MAX_TEXTURES = 6;
uniform sampler2D textures[MAX_TEXTURES];
uniform usampler2D splatmap;

uniform vec3 fogColor;

#include "../lib/light_FS.glsl"

uniform Light lights[MAX_LIGHTS];

void main() 
{
	// splatmap color
	uint splatMask = texture(splatmap, fTexCoords).r;
	
	vec2 nTexCoords = fTexCoords * 40.0f;
	
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
	
	Material totalMat = intersectAllLights(lights, normalEye, viewDirN);
	
	// modulate with light
	totalMat.ambient *= totalColor.xyz;
	totalMat.diffuse *= totalColor.xyz;
	totalMat.specular *= totalColor.xyz;
   
    fColor = vec4(min(totalMat.ambient + totalMat.diffuse + totalMat.specular, 1.0f), 1.0f);
	fColor = mix(vec4(fogColor, 1.0f), fColor, fVisibility);
}
