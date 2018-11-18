#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 Normal;
out vec2 TexCoord;
out vec3 WorldPos;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	Normal = (mat3(transpose(inverse(model))) * aNormal);
	TexCoord = aTexCoord;
	WorldPos = (model * vec4(aPos, 1.0)).xyz;
}
