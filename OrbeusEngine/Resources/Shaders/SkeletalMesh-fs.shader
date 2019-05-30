#version 330

out vec4 FragColor;
in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPos;
in vec3 Color;


void main()
{
	//FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	FragColor = vec4(Color, 1.0f);
}
