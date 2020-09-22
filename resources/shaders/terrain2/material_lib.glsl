struct Material
{
    sampler2D diffuseMap;
    sampler2D normalMap;
    sampler2D displaceMap;
    float heightScaling;
    float horizontalScaling;
};
const int MAX_MATERIALS = 5;
uniform Material materials[MAX_MATERIALS];