#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
//layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 clipSpace;
out vec3 toCameraVector;
out vec3 fromLightVector;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec3 cameraPos;
uniform vec3 lightPosition;

const float tiling = 4.0f;

void main()
{
	vec4 worldPos = model * vec4(aPos, 1.0);
	gl_Position = projection * view * worldPos;

	clipSpace = gl_Position;

	TexCoord = vec2(aPos.x / 2.0f + 0.5f, aPos.z / 2.0f + 0.5f) * tiling;
	toCameraVector = cameraPos - worldPos.xyz;
	fromLightVector = worldPos.xyz - lightPosition;
}
