struct Material
{
    sampler2D diffuseMap;
    sampler2D normalMap;
    float horizontalScaling;
};
const int MAX_MATERIALS = 5;
uniform Material materials[MAX_MATERIALS];