#version 430

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in mat4 gModelView[];
in float gLifePercentage[];

out vec2 fTexCoords;
out float fLifePercentage;

uniform mat4 uProjection;

const vec3 nUp = vec3(0, 0.5f, 0);
const vec3 nRight = vec3(0.5f, 0, 0);

void GenerateVertex(vec3 center, mat4 MVP, vec3 yAxis, vec3 xAxis, vec2 texCoords) {
    gl_Position = MVP * vec4(center + yAxis + xAxis, 1.0f);
    fTexCoords = texCoords;
    fLifePercentage = gLifePercentage[0];
    EmitVertex();
}

void main() {
    mat4 MVP = uProjection * gModelView[0];
    vec3 center = vec3(gl_in[0].gl_Position);

    GenerateVertex(center, MVP, nUp, -nRight, vec2(0, 0));  // TOP LEFT
    GenerateVertex(center, MVP, -nUp, -nRight, vec2(1, 0)); // BOTTOM LEFT
    GenerateVertex(center, MVP, nUp, nRight, vec2(0, 1));   // TOP RIGHT
    GenerateVertex(center, MVP, -nUp, nRight, vec2(1, 1));  // BOTTOM RIGHT

    EndPrimitive();
}