#version 330

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoord;

uniform vec2 translation;

void main()
{
	gl_Position = vec4(aPos.x + translation.x - 1, aPos.y - translation.y + 1, 0.0, 1.0);
	TexCoord = aTexCoords;
}
