#version 330

out vec4 FragColor;
in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPos;
in vec3 Color;

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

uniform sampler2D texture_diffuse;
uniform Material material;

void main()
{
	// Calculate material colors
	vec3 diffuseColor = material.diffuse;
	if (material.useDiffuseTexture)
	{
		diffuseColor = vec3(texture(material.texture_diffuse, TexCoord));
	}

	//FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	FragColor = vec4(Color, 1.0f);
	FragColor = vec4(diffuseColor, 1.0f);
	//FragColor = vec4(TexCoord.x, 0.0f, 0.0f, 1.0f);
}
