#version 430 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, rgba32f) uniform writeonly image2D splatMap;

uniform sampler2D heightMap;
uniform sampler2D normalMap;

uniform int size;

struct Biome {
    float minHeight;
    float maxHeight;
    float minSlope;
    float maxSlope;
};
const int BIOME_COUNT = VDGE_BIOMES_COUNT;

uniform Biome biomes[BIOME_COUNT];

void main() {
    ivec2 x = ivec2(gl_GlobalInvocationID.xy);
    vec2 texCoord = gl_GlobalInvocationID.xy / float(size);

    float height = texture(heightMap, texCoord).r; // * scaleY;

    vec3 normal = normalize(texture(normalMap, texCoord).rgb);
    float slope = normal.z;

    float[16] blendValues = float[](0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    float total = 0;
    for (int i = 0; i < BIOME_COUNT; ++i) {
        if (height >= biomes[i].minHeight && height < biomes[i].maxHeight && 
            slope >= biomes[i].minSlope && slope < biomes[i].maxSlope) {
            blendValues[i] = 1;
            total = total + 1;
        }
    }

    for (int i = 0; i < 16; ++i) {
        blendValues[i] = blendValues[i] / total;
    }

    vec4 blendSampleTL = vec4(blendValues[0], blendValues[1], blendValues[2], blendValues[3]);
    vec4 blendSampleTR = vec4(blendValues[4], blendValues[5], blendValues[6], blendValues[7]);
    vec4 blendSampleBL = vec4(blendValues[8], blendValues[9], blendValues[10], blendValues[11]);
    vec4 blendSampleBR = vec4(blendValues[12], blendValues[13], blendValues[14], blendValues[15]);

    ivec2 sCoords = ivec2(x.x >> 1, x.y >> 1);

    int sSize = size >> 1;

    imageStore(splatMap, sCoords, blendSampleTL);
    imageStore(splatMap, ivec2(sCoords.x + sSize, sCoords.y), blendSampleTR);
    imageStore(splatMap, ivec2(sCoords.x, sCoords.y + sSize), blendSampleBL);
    imageStore(splatMap, ivec2(sCoords.x + sSize, sCoords.y + sSize), blendSampleBR);
}