#version 330

#include Lighting-header.shader

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;

	vec3 diffuse;
	vec3 specular;

	bool useDiffuseTexture;
	bool useSpecularTexture;

	float specularIntensity;
	float specularPower;
};

out vec4 FragColor;
in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPos;

uniform sampler2D texture_diffuse;
uniform Material material;

void main()
{
	// For transparency
	if (texture(material.texture_diffuse, TexCoord).a < 0.5f)
	{
		discard;
	}


	// Calculate material colors
	vec3 diffuseColor = material.diffuse;
	if (material.useDiffuseTexture)
	{
		diffuseColor = vec3(texture(material.texture_diffuse, TexCoord));
	}

	vec3 specularColor = material.specular;
	if (material.useSpecularTexture)
	{
		specularColor = vec3(texture(material.texture_specular, TexCoord));
	}

	LightResult light = calcLights(Normal, WorldPos);

	vec3 ambient = light.ambient * diffuseColor;
	vec3 diffuse = light.diffuse * diffuseColor;
	vec3 specular = light.specular * specularColor;

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}
