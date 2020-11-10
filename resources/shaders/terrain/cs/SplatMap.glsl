#version 430 core

layout(local_size_x = 16, local_size_y = 16) in;

layout(binding = 0, r32ui) uniform writeonly uimage2D splatMap;

uniform sampler2D heightMap;
uniform float scaleY;
uniform int size;

struct Biome {
    float minHeight;
    float maxHeight;
};
const int BIOME_COUNT = 5;

uniform Biome biomes[BIOME_COUNT];

void main() {
    ivec2 x = ivec2(gl_GlobalInvocationID.xy);
    vec2 texCoord = gl_GlobalInvocationID.xy / float(size);

    float h = (texture(heightMap, texCoord).r) * scaleY;

    uint mask = 0;
    for (int k = 0; k < BIOME_COUNT; ++k) {
        if (biomes[k].minHeight <= h && h <= biomes[k].maxHeight) {
            // set bit on position K to mark that we use the k-th texture
            mask |= uint(1 << k);
        }
    }

    while (mask == 0) break;

    imageStore(splatMap, x, ivec4(mask, 0, 0, 0));
}