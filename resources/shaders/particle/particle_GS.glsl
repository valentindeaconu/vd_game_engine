#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in mat4 gModel[];
in float gLifePercentage[];

out vec2 fTexCoords;
out float fLifePercentage;

uniform mat4 uView;
uniform mat4 uProjection;

uniform vec3 uCameraUp;
uniform vec3 uCameraRight;

void GenerateVertex(vec3 center, mat4 VP, vec3 yAxis, vec3 xAxis, vec2 texCoords) {
    vec3 position = center + yAxis + xAxis;

    gl_Position = VP * gModel[0] * vec4(position, 1.0f);
    fTexCoords = texCoords;
    fLifePercentage = gLifePercentage[0];
    EmitVertex();
}

void main() {
    vec3 nUp = normalize(uCameraUp) * 0.5f;
    vec3 nRight = normalize(uCameraRight) * 0.5f;

    mat4 VP = uProjection * uView;
    vec3 center = vec3(gl_in[0].gl_Position);

    GenerateVertex(center, VP, nUp, -nRight, vec2(0, 0));  // TOP LEFT
    GenerateVertex(center, VP, -nUp, -nRight, vec2(1, 0)); // BOTTOM LEFT
    GenerateVertex(center, VP, nUp, nRight, vec2(0, 1));   // TOP RIGHT
    GenerateVertex(center, VP, -nUp, nRight, vec2(1, 1));  // BOTTOM RIGHT

    EndPrimitive();
}