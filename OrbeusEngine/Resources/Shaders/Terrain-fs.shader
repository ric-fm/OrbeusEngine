#version 330

#include Lighting-header.shader

out vec4 FragColor;
in vec3 Normal;
in vec2 TexCoord;
in vec3 WorldPos;
in float visibility;

uniform sampler2D background_texture;
uniform sampler2D red_texture;
uniform sampler2D green_texture;
uniform sampler2D blue_texture;
uniform sampler2D blend_map;

uniform vec3 fogColor;

vec3 calcMultiTextureColor(sampler2D background_texture, sampler2D red_texture, sampler2D green_texture, sampler2D blue_texture, sampler2D blend_map, vec2 texCoord, float tiling)
{
	vec4 blendColor = texture(blend_map, texCoord);
	vec2 tiledTexCoord = texCoord * tiling;

	vec4 backgroundColor = texture(background_texture, tiledTexCoord) * (1 - (blendColor.r + blendColor.g + blendColor.b));
	vec4 rColor = texture(red_texture, tiledTexCoord) * blendColor.r;
	vec4 gColor = texture(green_texture, tiledTexCoord) * blendColor.g;
	vec4 bColor = texture(blue_texture, tiledTexCoord) * blendColor.b;

	return (backgroundColor + rColor + gColor + bColor).xyz;
}

void main()
{
	vec3 color = calcMultiTextureColor(background_texture, red_texture, green_texture, blue_texture, blend_map, TexCoord, 100.0f);

	LightResult light = calcLights(Normal, WorldPos);

	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * color;

	FragColor = vec4(ambient + diffuse, 1.0f);
	FragColor = mix(vec4(fogColor, 1.0f), FragColor, visibility);
}
