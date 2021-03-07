#version 430

layout (points) in;
layout(triangle_strip, max_vertices = 4) out;

in vec2 gTexCoords[];

out vec2 fTexCoords;

uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraUp;
uniform vec3 cameraRight;

const float dist = 200.0f;

// const vec2 offset = vec2(0.05f, 0.05f);

void GenerateVertex(vec3 center, mat4 VP, vec3 yAxis, vec3 xAxis, vec2 texCoords) {
    gl_Position = VP * vec4(center + yAxis * dist + xAxis * dist, 1.0f);
    // gl_Position /= gl_Position.w;
    // gl_Position.xy += (texCoords - 0.5f) * offset;
    fTexCoords = texCoords;
    EmitVertex();
}

void main() {
    vec3 nUp = normalize(cameraUp);
    vec3 nRight = normalize(cameraRight);

    mat4 VP = projection * view;

    vec3 center = vec3(gl_in[0].gl_Position);

    GenerateVertex(center, VP, nUp, -nRight, vec2(0.0f, 0.0f)); // TOP LEFT
    GenerateVertex(center, VP, -nUp, -nRight, vec2(0.0f, 1.0f)); // BOTTOM LEFT
    GenerateVertex(center, VP, nUp, nRight, vec2(1.0f, 0.0f)); // TOP RIGHT
    GenerateVertex(center, VP, -nUp, nRight, vec2(1.0f, 1.0f)); // BOTTOM RIGHT

    /*// top left
    gl_Position = VP * vec4(center + nUp * dist - nRight * dist, 1.0f);
    fTexCoords = vec2(0.0f, 0.0f);
    EmitVertex();

    // bottom left
    gl_Position = VP * vec4(center - nUp * dist - nRight * dist, 1.0f);
    fTexCoords = vec2(0.0f, 1.0f);
    EmitVertex();

    // top right
    gl_Position = VP * vec4(center + nUp * dist + nRight * dist, 1.0f);
    fTexCoords = vec2(1.0f, 0.0f);
    EmitVertex();

    // bottom right
    gl_Position = VP * vec4(center - nUp * dist + nRight * dist, 1.0f);
    fTexCoords = vec2(1.0f, 1.0f);
    EmitVertex();*/

    EndPrimitive();
}