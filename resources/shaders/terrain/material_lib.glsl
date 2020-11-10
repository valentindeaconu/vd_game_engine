struct TerrainMaterial
{
    sampler2D diffuseMap;
    sampler2D normalMap;
    sampler2D displaceMap;
    float horizontalScaling;
    float heightScaling;
};
const int MAX_MATERIALS = 5;
uniform TerrainMaterial materials[MAX_MATERIALS];