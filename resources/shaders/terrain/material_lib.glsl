struct TerrainMaterial
{
    sampler2D diffuseMap;
    sampler2D normalMap;
    sampler2D displaceMap;
    float horizontalScaling;
    float heightScaling;
};
const int MAX_MATERIALS = VDGE_BIOMES_COUNT;
uniform TerrainMaterial materials[MAX_MATERIALS];

float[16] BlendSample(sampler2D splatMap, vec2 uv) {
    vec2 sUV = uv * 0.5f;

    vec4 blendSampleTL = texture(splatMap, sUV);
    vec4 blendSampleTR = texture(splatMap, vec2(sUV.x + 0.5f, sUV.y));
    vec4 blendSampleBL = texture(splatMap, vec2(sUV.x, sUV.y + 0.5f));
    vec4 blendSampleBR = texture(splatMap, vec2(sUV.x + 0.5f, sUV.y + 0.5f));

    float[16] blendSampleArray = float[](
        blendSampleTL.r, blendSampleTL.g, blendSampleTL.b, blendSampleTL.a,
        blendSampleTR.r, blendSampleTR.g, blendSampleTR.b, blendSampleTR.a,
        blendSampleBL.r, blendSampleBL.g, blendSampleBL.b, blendSampleBL.a,
        blendSampleBR.r, blendSampleBR.g, blendSampleBR.b, blendSampleBR.a
    );

    return blendSampleArray;
}