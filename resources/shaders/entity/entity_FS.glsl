#version 410 core

in vec4 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

in float fVisibility;

out vec4 fColor;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform int fakeLighting = 0;
uniform int transparency = 1;

uniform mat3 normalMatrix;

#include <light.glsl>
#include <fog.glsl>

void main() 
{	
	vec4 diffuseColor = texture(diffuseMap, fTexCoords);

	if (transparency == 1 && diffuseColor.a < 0.25f) {
		discard;
	}

	vec3 normal = fNormal;
	if (fakeLighting == 1) {
		normal = vec3(0.0f, 1.0f, 0.0f);
	}

	vec4 specularColor = texture(specularMap, fTexCoords);

	//in eye coordinates, the viewer is situated at the origin
	vec3 cameraPosEye = vec3(0.0f);
	//transform normal
	vec3 normalEye = normalize(normalMatrix * normal);
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosition.xyz);

	//modulate with lights
	Material material;
	material.Ambient = diffuseColor.xyz;
	material.Diffuse = diffuseColor.xyz;
	material.Specular = specularColor.xyz;

	vec3 lighting = modulateWithLightsAndShadow(sun, lights, normalEye, viewDirN, lightDirectionMatrix, fPosition.xyz, material, 0.0f);

	//combine results
	fColor = vec4(mix(fog.Color, lighting, fVisibility), diffuseColor.a);
}
