#version 330

out vec4 FragColor;

struct BaseLight
{
	vec3 color;
	float intensity;
};

struct DirectionalLight
{
	BaseLight base;
	vec3 direction;
};



in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture_diffuse;

uniform vec3 ambientLight;
uniform DirectionalLight directionalLight;


vec4 calcLight(BaseLight base, vec3 direction, vec3 normal)
{
	float diffuseFactor = max(dot(normal, normalize(-direction)), 0.0);

	vec4 diffuseColor = vec4(base.color, 1.0) * base.intensity * diffuseFactor;

	return diffuseColor;
}

vec4 calcDirectionalLight(DirectionalLight directionalLight, vec3 normal)
{
	return calcLight(directionalLight.base, directionalLight.direction, normal);
}

void main()
{
	vec4 light = vec4(ambientLight, 1.0);
	
	light += calcDirectionalLight(directionalLight, Normal);

	FragColor = texture(texture_diffuse, TexCoord) * light;
}
