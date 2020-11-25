struct Fog {
	float Density;
	float SkyDensity;
	float Gradient;
	vec3 Color;
};

uniform Fog fog;

float GetVisibilityThruFog(vec3 cameraPosition, float fogDensity, float fogGradient) {
	float dist = length(cameraPosition);
	float visibility = exp(-pow(dist * fogDensity, fogGradient));
	visibility = clamp(visibility, 0.0f, 1.0f);
	return visibility;
}