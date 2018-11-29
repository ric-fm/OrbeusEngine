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
out float visibility;

uniform float fogDensity;
const float fogGradient = 1.0f;

uniform vec4 clipPlane;

void main()
{
	vec4 worldPos = model * vec4(aPos, 1.0f);
	gl_ClipDistance[0] = dot(worldPos, clipPlane);

	vec4 posRelativeToCamera = view * model * vec4(aPos, 1.0f);	
	gl_Position = projection * posRelativeToCamera;

	Normal = (mat3(transpose(inverse(model))) * aNormal);
	TexCoord = aTexCoord;
	WorldPos = (model * vec4(aPos, 1.0)).xyz;
	
	// Fog calculation
	float distance = length(posRelativeToCamera.xyz);
	visibility = clamp(exp(-pow((distance*fogDensity), fogGradient)), 0.0f, 1.0f);
}
