#version 410 core

in vec4 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

in float fVisibility;

out vec4 fColor;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

uniform int transparency = 1;

#include <light.glsl>
#include <fog.glsl>

void main() 
{	
	vec4 diffuseColor = texture(diffuseMap, fTexCoords);

	if (transparency == 1 && diffuseColor.a < 0.25f) {
		discard;
	}

	vec4 specularColor = texture(specularMap, fTexCoords);

	vec3 viewDirN = normalize(-fPosition.xyz);

	//modulate with lights
	Material material;
	material.Ambient = diffuseColor.xyz;
	material.Diffuse = diffuseColor.xyz;
	material.Specular = specularColor.xyz;

	vec3 lighting = modulateWithLightsAndShadow(sun, lights, fNormal, viewDirN, fPosition.xyz, material, 0.0f);

	//combine results
	fColor = vec4(mix(fog.Color, lighting, fVisibility), diffuseColor.a);
}
