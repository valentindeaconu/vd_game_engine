
const int MAX_LIGHTS = 2;

struct Light
{
	vec3 position;
	vec3 color;
	vec3 attenuation;
	float ambientStrength;
	float specularStrength;
	float shininess;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

Material computeLight(Light light, vec3 normalEye, vec3 viewDirN)
{
	Material material;

	//compute light position in eye coordinates
	vec4 lightPosEye = view * vec4(light.position, 1.0f);

	//compute distance to light
	float dist = length(lightPosEye.xyz - fPosition.xyz);

	//compute attenuation
	float attFactor = 1.0f / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));

	//compute light direction
	vec3 lightDir = normalize(lightPosEye.xyz - fPosition.xyz);

	//compute ambient light
	material.ambient +=  attFactor * light.ambientStrength * light.color;

	//compute diffuse light
	material.diffuse += attFactor * max(dot(normalEye, lightDir), 0.0f) * light.color;

	//compute specular light
	vec3 reflection = reflect(-lightDir, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), light.shininess);
	
	material.specular += attFactor * light.specularStrength * specCoeff * light.color;
	
	return material;
}

Material intersectAllLights(Light lights[MAX_LIGHTS], vec3 normalEye, vec3 viewDirN)
{
	Material sum;
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		Material curr = computeLight(lights[i], normalEye, viewDirN);
		
		sum.ambient += curr.ambient;
		sum.diffuse += curr.diffuse;
		sum.specular += curr.specular;
	}
	
	return sum;
}