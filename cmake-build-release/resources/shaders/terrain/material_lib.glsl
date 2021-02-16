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