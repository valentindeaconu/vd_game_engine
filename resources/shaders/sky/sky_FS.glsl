#version 410 core

in vec3 fPosition;
in float fVisibility;

layout(location = 0) out vec4 fColor;

uniform vec3 color;
uniform vec3 colorFactor;

#include <fog.glsl>

vec3 ComputeColor(vec3 factor, vec3 color) {
	float red = factor.r * (abs(fPosition.y) - 2800.0f) + color.r;
	float green = factor.g * (abs(fPosition.y) - 2800.0f) + color.g;
	float blue = factor.b * (abs(fPosition.y) - 2800.0f) + color.b;

	return vec3(red, green, blue);
}

void main() {
	vec3 color = ComputeColor(colorFactor, color);

	fColor = vec4(color, 1.0f);
	fColor = mix(vec4(fog.Color, 1.0f), fColor, fog.SkyDensity);
}