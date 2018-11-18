
const int MAX_DIRECTIONAL_LIGHTS = 4;
const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};

struct Attenuation
{
	float constant;
	float linear;
	float exponential;
};

struct PointLight
{
	BaseLight base;
	vec3 position;

	Attenuation attenuation;
	float radius;
};

struct SpotLight
{
	PointLight pointLight;
	vec3 direction;
	float cutoff;
};

struct LightResult
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

LightResult calcLight(BaseLight base, vec3 direction, vec3 normal, vec3 viewPos, vec3 worldPos, float specularIntensity, float specularPower)
{
	LightResult result;

	vec3 lightDirection = normalize(-direction);

	// Diffuse
	float diffuseFactor = max(dot(normal, lightDirection), 0.0);
	result.diffuse = base.color * base.intensity * diffuseFactor;

	//// Specular
	//vec3 viewDirection = normalize(viewPos - worldPos);
	//vec3 reflectDirection = reflect(-lightDirection, normal);
	//float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), specularPower);
	//result.specular = base.color * specularIntensity * specularFactor;

	// Blinn-Phong Specular
	vec3 viewDirection = normalize(viewPos - worldPos);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), specularPower);
	result.specular = base.color * specularIntensity * specularFactor;

	return result;
}

LightResult calcDirectionalLight(DirectionalLight directionalLight, vec3 normal, vec3 viewPos, vec3 worldPos, float specularIntensity, float specularPower)
{
	return calcLight(directionalLight.base, directionalLight.direction, normal, viewPos, worldPos, specularIntensity, specularPower);
}

LightResult calcPointLight(PointLight pointLight, vec3 normal, vec3 viewPos, vec3 worldPos, float specularIntensity, float specularPower)
{
	vec3 lightDirection = normalize(worldPos - pointLight.position);
	float distanceToPointLight = length(worldPos - pointLight.position);

	LightResult result;
	if (distanceToPointLight <= pointLight.radius)
	{
		float attenuation = 1.0 / (0.00001 +	// Prevent division by 0
			pointLight.attenuation.constant +
			pointLight.attenuation.linear * distanceToPointLight +
			pointLight.attenuation.exponential * distanceToPointLight * distanceToPointLight);

		result = calcLight(pointLight.base, lightDirection, normal, viewPos, worldPos, specularIntensity, specularPower);
		result.diffuse *= attenuation;
		result.specular *= attenuation;
	}

	return result;
}

LightResult calcSpotLight(SpotLight spotLight, vec3 normal, vec3 viewPos, vec3 worldPos, float specularIntensity, float specularPower)
{
	vec3 lightDirection = normalize(worldPos - spotLight.pointLight.position);
	float spotFactor = dot(lightDirection, spotLight.direction);

	LightResult result;

	if (spotFactor > spotLight.cutoff)
	{
		result = calcPointLight(spotLight.pointLight, normal, viewPos, worldPos, specularIntensity, specularPower);

		result.diffuse *= (1.0 - (1.0 - spotFactor) / (1.0 - spotLight.cutoff));
		result.specular *= (1.0 - (1.0 - spotFactor) / (1.0 - spotLight.cutoff));
	}

	return result;
}


// General data and method for calculate all lights (forward rendering)
uniform vec3 viewPos;
uniform vec3 ambientLight;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

LightResult calcLights(vec3 normal, vec3 worldPos)
{
	LightResult result;
	result.ambient = ambientLight;

	for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS; ++i)
	{
		if (directionalLights[i].base.intensity > 0.0)
		{
			LightResult directionalLight = calcDirectionalLight(directionalLights[i], normal, viewPos, worldPos, 0, 0);
			result.diffuse += directionalLight.diffuse;
			result.specular += directionalLight.specular;
		}
	}

	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if (pointLights[i].base.intensity > 0.0)
		{
			LightResult pointLight = calcPointLight(pointLights[i], normal, viewPos, worldPos, 0, 0);
			result.diffuse += pointLight.diffuse;
			result.specular += pointLight.specular;
		}
	}

	for (int i = 0; i < MAX_SPOT_LIGHTS; ++i)
	{
		if (spotLights[i].pointLight.base.intensity > 0.0)
		{
			LightResult spotLight = calcSpotLight(spotLights[i], normal, viewPos, worldPos, 0, 0);
			result.diffuse += spotLight.diffuse;
			result.specular += spotLight.specular;
		}
	}
	return result;
}
