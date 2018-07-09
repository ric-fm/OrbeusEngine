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
uniform PointLight pointLights[MAX_POINT_LIGHTS];

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

	if (distanceToPointLight < pointLight.radius)
	{
		LightResult result;
		result.diffuse = vec3(0.0);
		result.specular = vec3(0.0);
		return result;
	}

	float attenuation = 1.0 / ( 0.00001 +	// Prevent division by 0
			pointLight.attenuation.constant +
			pointLight.attenuation.linear * distanceToPointLight +
			pointLight.attenuation.exponential * distanceToPointLight * distanceToPointLight);

	LightResult result = calcLight(pointLight.base, lightDirection, normal, specularIntensity, specularPower);
	result.diffuse *= attenuation;
	result.specular *= attenuation;

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

	vec3 ambient = ambientLight * vec3(texture(material.texture_diffuse, TexCoord));
	vec3 diffuse = light.diffuse * vec3(texture(material.texture_diffuse, TexCoord));
	vec3 specular = light.specular * vec3(texture(material.texture_specular, TexCoord));

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
