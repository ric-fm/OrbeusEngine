#version 330

out vec4 FragColor;
in vec3 TexCoord;

uniform samplerCube skybox;
uniform vec3 fogColor;

const float lowerLimit = 0.0f;
const float upperLimit = 0.3f;

void main()
{
	FragColor = texture(skybox, TexCoord);

	float factor = (TexCoord.y - lowerLimit) / (upperLimit - lowerLimit);
	factor = clamp(factor, 0.0f, 1.0f);
	FragColor = mix(vec4(fogColor, 1.0f), FragColor, factor);
}
