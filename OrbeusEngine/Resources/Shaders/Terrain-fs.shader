#version 330

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse;

void main()
{
	FragColor = texture(texture_diffuse, TexCoord);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
