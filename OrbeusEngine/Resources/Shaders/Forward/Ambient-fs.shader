#version 330

out vec4 FragColor;

struct Material
{
	sampler2D texture_diffuse;
};

uniform vec3 ambientLight;
uniform Material material;

in vec2 TexCoord;

void main()
{
	FragColor = texture(material.texture_diffuse, TexCoord) * vec4(ambientLight, 1.0f);
}
