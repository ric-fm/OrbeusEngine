#version 330

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D atlas;
uniform vec3 color;

const float width = 0.5;
const float edge = 0.1;

void main()
{
	float distance = 1.0 - texture(atlas, TexCoord).a;
	float alpha = 1.0 - smoothstep(width, width + edge, distance);

	FragColor = vec4(color, alpha);
}
