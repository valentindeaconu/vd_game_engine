#version 410 core

in vec4 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

in mat3 fNormalMatrix;
in mat3 fLightDirectionMatrix;
in float fVisibility;

out vec4 fColor;

uniform mat4 view;

uniform sampler2D diffusemap;
uniform sampler2D specularmap;

uniform vec3 fogColor;

#include "../lib/light_FS.glsl"

uniform Light lights[MAX_LIGHTS];

void main() 
{	
	vec4 diffuseColor = texture(diffusemap, fTexCoords);
	
	if (diffuseColor.a < 0.01f)
	{
		discard;
	}
	
	//in eye coordinates, the viewer is situated at the origin
	vec3 cameraPosEye = vec3(0.0f);
	//transform normal
	vec3 normalEye = normalize(fNormalMatrix * fNormal);	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosition.xyz);
	
	//intersect with all lights
	Material totalMat = intersectAllLights(lights, normalEye, viewDirN);
	
	//modulate with diffuse map
	totalMat.ambient *= diffuseColor.xyz;
	totalMat.diffuse *= diffuseColor.xyz;
	
	//modulate woth specular map
	vec4 specularColor = texture(specularmap, fTexCoords);
	totalMat.specular *= specularColor.xyz;
   
    fColor = vec4(min(totalMat.ambient + totalMat.diffuse + totalMat.specular, 1.0f), diffuseColor.a);
	fColor = mix(vec4(fogColor, 1.0f), fColor, fVisibility);
}
