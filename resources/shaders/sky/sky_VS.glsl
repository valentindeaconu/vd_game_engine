#version 410 core
// #version 330

layout (location = 0) in vec3 vPosition;

out vec3 fPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 skyPosition = projection * view * vec4(vPosition, 1.0f);

	gl_Position = skyPosition.xyww;

	fPosition = vPosition;
}
