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

uniform float specularIntensity;
uniform float specularPower;

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

void main()
{
	LightResult light = calcDirectionalLight(directionalLight, Normal, material.specularIntensity, material.specularPower);

	vec3 ambient = ambientLight * vec3(texture(material.texture_diffuse, TexCoord));
	vec3 diffuse = light.diffuse * vec3(texture(material.texture_diffuse, TexCoord));
	vec3 specular = light.specular * vec3(texture(material.texture_specular, TexCoord));

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
