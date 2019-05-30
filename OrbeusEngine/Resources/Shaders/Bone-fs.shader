#version 330

#include Lighting-header.shader

out vec4 FragColor;
in vec3 Normal;
in vec3 WorldPos;

uniform vec3 color;

void main()
{
	LightResult light = calcLights(Normal, WorldPos);
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * color;
	vec3 specular = light.specular * color;
	FragColor = vec4(ambient + diffuse + specular, 1.0f);

	//FragColor = vec4(color, 1.0);
}
