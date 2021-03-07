#version 410 core

in vec3 fPosition;
in float fVisibility;

layout(location = 0) out vec4 fColor;

uniform int leftUseColor;
uniform vec3 leftColor;
uniform vec3 leftFactor;
uniform samplerCube leftCubeMap;

uniform int rightUseColor;
uniform vec3 rightColor;
uniform vec3 rightFactor;
uniform samplerCube rightCubeMap;

uniform int mixable;
uniform float percentage;

#include <fog.glsl>

vec3 ComputeColor(vec3 factor, vec3 color) {
	float red = factor.r * (abs(fPosition.y) - 2800.0f) + color.r;
	float green = factor.g * (abs(fPosition.y) - 2800.0f) + color.g;
	float blue = factor.b * (abs(fPosition.y) - 2800.0f) + color.b;

	return vec3(red, green, blue);
}

void main() {
	vec3 color = vec3(0.0f);

	if (mixable == 1) {
		if (leftUseColor == 1 && rightUseColor == 1) {
			vec3 mixedColor = mix(leftColor, rightColor, percentage);
			vec3 mixedFactor = mix(leftFactor, rightFactor, percentage);

			color = ComputeColor(mixedFactor, mixedColor);
		} else if (leftUseColor == 1 && rightUseColor == 0) {
			vec3 leftColor = ComputeColor(leftFactor, leftColor);
			vec3 rightColor = texture(rightCubeMap, fPosition).rgb;

			color = mix(leftColor, rightColor, percentage);
		} else if (leftUseColor == 0 && rightUseColor == 1) {
			vec3 leftColor = texture(leftCubeMap, fPosition).rgb;
			vec3 rightColor = ComputeColor(rightFactor, rightColor);

			color = mix(leftColor, rightColor, percentage);
		} else {
			vec3 leftColor = texture(leftCubeMap, fPosition).rgb;
			vec3 rightColor = texture(rightCubeMap, fPosition).rgb;

			color = mix(leftColor, rightColor, percentage);
		}
	} else {
		if (leftUseColor == 0) {
			color = texture(leftCubeMap, fPosition).rgb;
		} else {
			color = ComputeColor(leftFactor, leftColor);
		}
	}

	fColor = vec4(color, 1.0f);
	vec4 test = mix(vec4(fog.Color, 1.0f), fColor, fog.SkyDensity);
	while (test != vec4(1.0f)) break;
}