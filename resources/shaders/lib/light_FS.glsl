
const int MAX_LIGHTS = 1;

struct Light {
	int type;
	vec3 direction;
	vec3 position;
	vec3 color;
	vec3 attenuation;
	float ambientStrength;
	float specularStrength;
	float shininess;
};

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

Material computePointLight(Light light, vec3 normalEye, vec3 viewDirN, vec3 fragPos) {
	Material material;

	//compute light position in eye coordinates
	vec4 lightPosEye = view * vec4(light.position, 1.0f);

	//compute distance to light
	float dist = length(lightPosEye.xyz - fragPos);

	//compute attenuation
	float attFactor = 1.0f / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));

	//compute light direction
	vec3 lightDir = normalize(lightPosEye.xyz - fragPos);

	//compute ambient light
	material.ambient =  attFactor * light.ambientStrength * light.color;

	//compute diffuse light
	material.diffuse = attFactor * max(dot(normalEye, lightDir), 0.0f) * light.color;

	//compute specular light
	vec3 reflection = reflect(-lightDir, normalEye);
	float specCoeff = pow(max(dot(viewDirN, reflection), 0.0f), light.shininess);
	
	material.specular = attFactor * light.specularStrength * specCoeff * light.color;
	
	return material;
}

Material computeDirectionalLight(Light light, vec3 normalEye, vec3 viewDirN, mat3 lightDirMatrix) {
	Material material;

	//compute light direction
	vec3 lightDirN = normalize(lightDirMatrix * light.direction);

	//compute ambient light
	material.ambient = light.ambientStrength * light.color;

	//compute diffuse light
	material.diffuse = max(dot(normalEye, lightDirN), 0.0f) * light.color;

	//compute specular light
	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), light.shininess);
	material.specular = light.specularStrength * specCoeff * light.color;

	return material;
}

vec3 modulateWithLightsAndShadow(Light sun,
								Light lights[MAX_LIGHTS],
								vec3 normalEye,
								vec3 viewDirN,
								mat3 lightDirMatrix,
								vec3 fragPos,
								Material material,
								float shadow) {
	Material total = computeDirectionalLight(sun, normalEye, viewDirN, lightDirMatrix);

	for (int i = 0; i < MAX_LIGHTS; ++i) {
		Material curr;

		if (lights[i].type == 0) {
			curr = computeDirectionalLight(lights[i], normalEye, viewDirN, lightDirMatrix);
		} else if (lights[i].type == 1) {
			curr = computePointLight(lights[i], normalEye, viewDirN, fragPos);
		} else {
			// TODO: Finish the spot light computation
		}

		total.ambient += curr.ambient;
		total.diffuse += curr.diffuse;
		total.specular += curr.specular;
	}

	total.ambient *= material.ambient;
	total.diffuse *= material.diffuse;
	total.specular *= material.specular;

	return min(total.ambient + (1.0f - shadow) * (total.diffuse + total.specular), 1.0f);
}