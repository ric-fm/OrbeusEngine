#version 330

out vec4 FragColor;

struct Material
{
	sampler2D texture_diffuse;

	vec3 diffuse;

	bool useDiffuseTexture;
};

uniform vec3 ambientLight;
uniform Material material;

in vec2 TexCoord;

void main()
{
	vec3 diffuseColor = material.diffuse;
	if (material.useDiffuseTexture)
	{
		diffuseColor = vec3(texture(material.texture_diffuse, TexCoord));
	}

	FragColor = vec4(diffuseColor * ambientLight, 1.0f);
}
