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
	vec3 diffuse;
	vec3 specular;
};

in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPos;

uniform sampler2D texture_diffuse;
uniform sampler2D texture_specular;

uniform Material material;

uniform vec3 ambientLight;
uniform DirectionalLight directionalLight;

const int MAX_POINT_LIGHTS = 4;
const int MAX_SPOT_LIGHTS = 4;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform vec3 viewPos;


LightResult calcLight(BaseLight base, vec3 direction, vec3 normal, float specularIntensity, float specularPower)
{
	LightResult result;

	vec3 lightDirection = normalize(-direction);

	// Diffuse
	float diffuseFactor = max(dot(normal, lightDirection), 0.0);
	result.diffuse = base.color * base.intensity * diffuseFactor;

	// Specular
	vec3 viewDirection = normalize(viewPos - WorldPos);
	vec3 reflectDirection = reflect(-lightDirection, normal);
	float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), specularPower);
	result.specular = base.color * specularIntensity * specularFactor;


	return result;
}

LightResult calcDirectionalLight(DirectionalLight directionalLight, vec3 normal, float specularIntensity, float specularPower)
{
	return calcLight(directionalLight.base, directionalLight.direction, normal, specularIntensity, specularPower);
}

LightResult calcPointLight(PointLight pointLight, vec3 normal, float specularIntensity, float specularPower)
{
	vec3 lightDirection = normalize(WorldPos - pointLight.position);
	float distanceToPointLight = length(WorldPos - pointLight.position);

	LightResult result;
	if (distanceToPointLight <= pointLight.radius)
	{
		float attenuation = 1.0 / ( 0.00001 +	// Prevent division by 0
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

	return result;
}

void main()
{
	LightResult light = calcDirectionalLight(directionalLight, Normal, material.specularIntensity, material.specularPower);

	for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
	{
		if (pointLights[i].base.intensity > 0.0)
		{
			LightResult pointLight = calcPointLight(pointLights[i], Normal, material.specularIntensity, material.specularPower);
			light.diffuse += pointLight.diffuse;
			light.specular += pointLight.specular;
		}
	}

	for (int i = 0; i < MAX_SPOT_LIGHTS; ++i)
	{
		if (spotLights[i].pointLight.base.intensity > 0.0)
		{
			LightResult spotLight = calcSpotLight(spotLights[i], Normal, material.specularIntensity, material.specularPower);
			light.diffuse += spotLight.diffuse;
			light.specular += spotLight.specular;
		}
	}

	vec3 ambient = ambientLight * vec3(texture(material.texture_diffuse, TexCoord));
	vec3 diffuse = light.diffuse * vec3(texture(material.texture_diffuse, TexCoord));
	vec3 specular = light.specular * vec3(texture(material.texture_specular, TexCoord));

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
