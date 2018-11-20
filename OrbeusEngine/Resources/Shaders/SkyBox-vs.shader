#version 330

layout(location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoord = aPos;
	// Take xyww to optimize depth test.
	// Set the z component to w. So the perspective division is applied and z translates to w/w= 1.0f.
	// The skybox is rendered as last entity to render the fragments that passes the early depth test.
	gl_Position = (projection * view * vec4(aPos, 1.0)).xyww;
}
