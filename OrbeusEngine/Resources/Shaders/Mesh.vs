#version 330

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 orb_Proj;
uniform mat4 orb_View;
uniform mat4 orb_Model;
uniform vec4 orb_ClipPlane;

out vec3 Normal;
out vec2 TexCoord;
out vec3 WorldPos;


void main()
{
	vec4 worldPos = orb_Model * vec4(aPos, 1.0f);
	gl_ClipDistance[0] = dot(worldPos, orb_ClipPlane);

	gl_Position = orb_Proj * orb_View * orb_Model * vec4(aPos, 1.0f);


	Normal = (mat3(transpose(inverse(orb_Model))) * aNormal);
	TexCoord = aTexCoord;
	WorldPos = (orb_Model * vec4(aPos, 1.0)).xyz;
}
