#version 410 core

in vec4 fPosition;
in vec2 fTexCoords;

in vec4 clipSpace;
in vec3 toCamera;
in vec3 fromLightVector;

out vec4 fColor;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

uniform sampler2D dudvMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform float tiling;
uniform float waveStrength;
uniform float moveFactor;
uniform float shineDamper;
uniform float reflectivity;
uniform vec3 sunColor;
uniform vec3 baseColor;
uniform float nearPlane;
uniform float farPlane;

float computeDistanceUsingDepth(float depth, float nearPlane, float farPlane) {
	return 2.0f * nearPlane * farPlane / (farPlane + nearPlane - (2.0f * depth - 1.0f) * (farPlane - nearPlane));
}

void main()
{
	// get reflection & refraction
	vec2 normCoords = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;

	vec2 refractionTexCoords = vec2(normCoords.x, normCoords.y);
	vec2 reflectionTexCoords = vec2(normCoords.x, -normCoords.y);

	float floorDistance = computeDistanceUsingDepth(texture(depthMap, refractionTexCoords).r, nearPlane, farPlane);
	float waterDistance = computeDistanceUsingDepth(gl_FragCoord.z, nearPlane, farPlane);
	float waterDepth = floorDistance - waterDistance;

	// compute distorsion
	vec2 distortedTexCoords = texture(dudvMap, vec2(fTexCoords.x + moveFactor, fTexCoords.y)).rg * 0.1f;
	distortedTexCoords = fTexCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);

	vec2 totalDistortion = (texture(dudvMap, distortedTexCoords).rg * 2.0 - 1.0) * waveStrength * clamp(waterDepth / 20.0f, 0.0f, 1.0f);

	refractionTexCoords += totalDistortion;
	refractionTexCoords = clamp(refractionTexCoords, 0.001f, 0.999f);

	reflectionTexCoords += totalDistortion;
	reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001f, 0.999f);
	reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999f, -0.001f);

	// get color from texture
	vec4 refractionColor = texture(refractionTexture, refractionTexCoords);
	vec4 reflectionColor = texture(reflectionTexture, reflectionTexCoords);

	// compute normal
	vec4 normalMapColor = texture(normalMap, distortedTexCoords);
	vec3 normal = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b * 5.0f, normalMapColor.g * 2.0f - 1.0f);
	normal = normalize(normal);

	// fesnel effect
	vec3 viewVector = normalize(toCamera);
	float refractiveFactor = dot(viewVector, normal);

	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0f);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = sunColor * specular * reflectivity * clamp(waterDepth / 5.0f, 0.0f, 1.0f);

	vec4 waterColor = mix(reflectionColor  + vec4(specularHighlights, 0.0f), refractionColor, refractiveFactor);
	waterColor = mix(waterColor, vec4(baseColor, 1.0f), 0.2f);

	fColor = mix(waterColor, vec4(baseColor, 1.0f), 0.35f);
	fColor.a = clamp(waterDepth / 5.0f, 0.0f, 1.0f);
}
