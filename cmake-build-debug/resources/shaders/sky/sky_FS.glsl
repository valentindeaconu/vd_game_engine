#version 410 core

in vec3 fPosition;
in float fVisibility;

layout(location = 0) out vec4 fColor;

const vec3 baseColor = vec3(0.18f, 0.27f, 0.47f);

#include <fog.glsl>

void main() {
	float red = -0.00022f * (abs(fPosition.y) - 2800.0f) + 0.18f;
	float green = -0.00025f * (abs(fPosition.y) - 2800.0f) + 0.27f;
	float blue = -0.00019f * (abs(fPosition.y) - 2800.0f) + 0.47f;

	fColor = vec4(red, green, blue, 1.0f);
	fColor = mix(vec4(fog.Color, 1.0f), fColor, fog.SkyDensity);
}