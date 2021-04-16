#version 410 core

in vec4 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

out vec4 fColor;

#include <material_fn.glsl>
#include <light_fn.glsl>
#include <fog_fn.glsl>

uniform MaterialMap uMaterialMap;
uniform int uTransparency;

uniform Light sun;
uniform Light lights[MAX_LIGHTS];

uniform Fog fog;

void main()
{
    Material material = SolveMaterialMap(uMaterialMap, fTexCoords);

    if (uTransparency == 1 && material.Diffuse.a < 0.25f) {
        discard;
    }

    Material lightMaterial = SumLights(sun, lights, fNormal, fPosition.xyz);
    Material withLightMaterial = ModulateMaterialWithLight(material, lightMaterial);
    vec4 color = ExtractColorFromMaterial(withLightMaterial, 1.0f);

    fColor = ModulateWithFog(fPosition.xyz, color, fog);
}
