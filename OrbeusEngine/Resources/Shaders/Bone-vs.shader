#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 boneTransform;

out vec3 Normal;
out vec2 TexCoord;
out vec3 WorldPos;

void main()
{
	Normal = (mat3(transpose(inverse(model))) * aNormal);
	WorldPos = (model * vec4(aPos, 1.0)).xyz;

	TexCoord = aTexCoord;

	gl_Position = projection * view * model * boneTransform * vec4(aPos, 1.0f);
}
