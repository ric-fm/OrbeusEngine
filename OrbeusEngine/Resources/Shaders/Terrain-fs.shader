#version 330

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D background_texture;
uniform sampler2D red_texture;
uniform sampler2D green_texture;
uniform sampler2D blue_texture;
uniform sampler2D blend_map;


void main()
{
	vec4 blendColor = texture(blend_map, TexCoord);

	float backTextureAmount = 1 - (blendColor.r + blendColor.g + blendColor.b);
	vec2 tiledTexCoord = TexCoord * 40.0;

	vec4 backgroundColor = texture(background_texture, tiledTexCoord) * backTextureAmount;
	vec4 rColor = texture(red_texture, tiledTexCoord) * blendColor.r;
	vec4 gColor = texture(green_texture, tiledTexCoord) * blendColor.g;
	vec4 bColor = texture(blue_texture, tiledTexCoord) * blendColor.b;

	FragColor = backgroundColor + rColor + gColor + bColor;
}
