#version 410 core

in vec3 fPosition;

layout(location = 0) out vec4 fColor;

uniform int leftUseColor;
uniform vec3 leftColor;
uniform vec3 leftFactor;
uniform vec2 leftFogLimits;
uniform samplerCube leftCubeMap;

uniform int rightUseColor;
uniform vec3 rightColor;
uniform vec3 rightFactor;
uniform vec2 rightFogLimits;
uniform samplerCube rightCubeMap;

uniform int mixable;
uniform float percentage;
uniform vec3 fogColor;

vec3 ComputeColor(vec3 factor, vec3 color) {
	float red = factor.r * (abs(fPosition.y) - 2800.0f) + color.r;
	float green = factor.g * (abs(fPosition.y) - 2800.0f) + color.g;
	float blue = factor.b * (abs(fPosition.y) - 2800.0f) + color.b;

	return vec3(red, green, blue);
}

vec3 ModulateWithFog(vec3 color, vec2 fogLimits) {
	float v = (fPosition.y + 1.0f) * 0.5f;
	float low = (fogLimits.x + 1.0f) * 0.5f;
	float high = (fogLimits.y + 1.0f) * 0.5f;
	float factor = clamp((v - low) / (high - low), 0.0f, 1.0f);
	
	return mix(fogColor, color, factor);
}

void main() {
	vec3 color = vec3(0.0f);

	if (mixable == 1) {
		if (leftUseColor == 1 && rightUseColor == 1) {
			vec3 l = ModulateWithFog(leftColor, leftFogLimits);
			vec3 r = ModulateWithFog(rightColor, rightFogLimits);

			vec3 mixedColor = mix(l, r, percentage);
			vec3 mixedFactor = mix(leftFactor, rightFactor, percentage);

			color = ComputeColor(mixedFactor, mixedColor);
		} else if (leftUseColor == 1 && rightUseColor == 0) {
			vec3 leftColor = ComputeColor(leftFactor, leftColor);
			vec3 rightColor = texture(rightCubeMap, fPosition).rgb;

			vec3 l = ModulateWithFog(leftColor, leftFogLimits);
			vec3 r = ModulateWithFog(rightColor, rightFogLimits);

			color = mix(l, r, percentage);
		} else if (leftUseColor == 0 && rightUseColor == 1) {
			vec3 leftColor = texture(leftCubeMap, fPosition).rgb;
			vec3 rightColor = ComputeColor(rightFactor, rightColor);

			vec3 l = ModulateWithFog(leftColor, leftFogLimits);
			vec3 r = ModulateWithFog(rightColor, rightFogLimits);

			color = mix(l, r, percentage);
		} else {
			vec3 leftColor = texture(leftCubeMap, fPosition).rgb;
			vec3 rightColor = texture(rightCubeMap, fPosition).rgb;

			vec3 l = ModulateWithFog(leftColor, leftFogLimits);
			vec3 r = ModulateWithFog(rightColor, rightFogLimits);

			color = mix(l, r, percentage);
		}
	} else {
		if (leftUseColor == 0) {
			color = texture(leftCubeMap, fPosition).rgb;
		} else {
			color = ComputeColor(leftFactor, leftColor);
		}

		color = ModulateWithFog(color, leftFogLimits);
	}

	fColor = vec4(color, 1.0f);
}