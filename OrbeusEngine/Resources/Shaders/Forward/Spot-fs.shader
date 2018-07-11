#version 330

out vec4 FragColor;

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;

	float specularIntensity;
	float specularPower;
};

struct BaseLight
{
	vec3 color;
	float intensity;
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
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPos;

uniform Material material;

uniform SpotLight spotLight;

uniform vec3 viewPos;

LightResult calcLight(BaseLight base, vec3 direction, vec3 normal, float specularIntensity, float specularPower)
{
	LightResult result;

	vec3 lightDirection = normalize(-direction);

	// Diffuse
	float diffuseFactor = max(dot(normal, lightDirection), 0.0);
	result.diffuse = base.color * base.intensity * diffuseFactor;

	//// Specular
	//vec3 viewDirection = normalize(viewPos - WorldPos);
	//vec3 reflectDirection = reflect(-lightDirection, normal);
	//float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), specularPower);
	//result.specular = base.color * specularIntensity * specularFactor;

	// Blinn-Phong Specular
	vec3 viewDirection = normalize(viewPos - WorldPos);
	vec3 halfwayDirection = normalize(lightDirection + viewDirection);
	float specularFactor = pow(max(dot(normal, halfwayDirection), 0.0), specularPower);
	result.specular = base.color * specularIntensity * specularFactor;


	return result;
}

LightResult calcPointLight(PointLight pointLight, vec3 normal, float specularIntensity, float specularPower)
{
	vec3 lightDirection = normalize(WorldPos - pointLight.position);
	float distanceToPointLight = length(WorldPos - pointLight.position);

	LightResult result;
	if (distanceToPointLight <= pointLight.radius)
	{
		float attenuation = 1.0 / (0.00001 +	// Prevent division by 0
			pointLight.attenuation.constant +
			pointLight.attenuation.linear * distanceToPointLight +
			pointLight.attenuation.exponential * distanceToPointLight * distanceToPointLight);

		result = calcLight(pointLight.base, lightDirection, normal, specularIntensity, specularPower);
		result.diffuse *= attenuation;
		result.specular *= attenuation;
	}

	return result;
}

LightResult calcSpotLight(SpotLight spotLight, vec3 normal, float specularIntensity, float specularPower)
{
	vec3 lightDirection = normalize(WorldPos - spotLight.pointLight.position);
	float spotFactor = dot(lightDirection, spotLight.direction);

	LightResult result;

	if (spotFactor > spotLight.cutoff)
	{
		result = calcPointLight(spotLight.pointLight, normal, specularIntensity, specularPower);

		result.diffuse *= (1.0 - (1.0 - spotFactor) / (1.0 - spotLight.cutoff));
		result.specular *= (1.0 - (1.0 - spotFactor) / (1.0 - spotLight.cutoff));
	}

	//result.diffuse = vec3(1);
	//result.specular = vec3(1);

	return result;
}

void main()
{
	LightResult light;

	if (spotLight.pointLight.base.intensity > 0.0)
	{
		light = calcSpotLight(spotLight, Normal, material.specularIntensity, material.specularPower);
	}

	vec3 diffuse = light.diffuse * vec3(texture(material.texture_diffuse, TexCoord));
	vec3 specular = light.specular * vec3(texture(material.texture_specular, TexCoord));

	FragColor = vec4(diffuse + specular, 1.0f);
}
