#version 330

const int MAX_BONES = 50;
const int MAX_WEIGHTS = 1;

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in vec3 aIndices;
layout(location = 4) in vec3 aWeights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 boneTransforms[MAX_BONES];

out vec3 Normal;
out vec2 TexCoord;
out vec3 Color;
//out vec3 WorldPos;

//uniform vec4 clipPlane;

void main()
{
	vec4 totalLocalPos = vec4(0.0); // vertex position in mode-space
	vec4 totalNormal = vec4(0.0);

	for (int i = 0; i < MAX_WEIGHTS; i++)
	{
		int boneIndex = int(aIndices[i]);
		mat4 boneTransform = boneTransforms[boneIndex];
		vec4 posePosition = boneTransform * vec4(aPos, 1.0);
		totalLocalPos += posePosition * aWeights[i];

		vec4 worldNormal = boneTransform * vec4(aNormal, 0.0);
		totalNormal += worldNormal * aWeights[i];
	}

	gl_Position = projection * view * model * vec4(aPos, 1.0);
	gl_Position = projection * view * model * totalLocalPos;

	Normal = totalNormal.xyz;
	TexCoord = aTexCoord;
	Color = aPos;
}
