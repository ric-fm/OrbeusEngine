#version 330

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse;

void main()
{
	FragColor = texture(texture_diffuse, TexCoord);
}
