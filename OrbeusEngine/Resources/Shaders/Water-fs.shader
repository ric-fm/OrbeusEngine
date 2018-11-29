#version 330

out vec4 FragColor;
in vec2 TexCoord;
in vec4 clipSpace;
in vec3 toCameraVector;
in vec3 fromLightVector;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D depthMap;
uniform sampler2D normalMap;
uniform sampler2D dudvMap;

uniform vec3 ligthColor;

uniform float moveFactor;

const float waveStrength = 0.04f;
const float shineDamper = 20.0f;
const float reflectivity = 0.5f;

void main()
{
	// Projection texturing
	vec2 ndc = (clipSpace.xy / clipSpace.w) / 2.0f + 0.5f;
	vec2 refractTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectTexCoords = vec2(ndc.x, -ndc.y);

	// Calculate the depth of the water (between the water pixel and the porjected refraction coordinates
	float near = 0.1f;
	float far = 1000.0f;
	float depth = texture(depthMap, refractTexCoords).r;
	float floorDistance = 2.0f * near * far / (far + near - (2.0f * depth - 1.0f) * (far - near));

	depth = gl_FragCoord.z;
	float waterDistance = 2.0f * near * far / (far + near - (2.0f * depth - 1.0f) * (far - near));

	float waterDepth = floorDistance - waterDistance;

	// Apply distorsion to texture coordinates
	vec2 distortedTexCoords = texture(dudvMap, vec2(TexCoord.x + moveFactor, TexCoord.y)).rg * 0.1f;
	distortedTexCoords = TexCoord + vec2(distortedTexCoords.x, distortedTexCoords.y + moveFactor);
	vec2 distorsion = (texture(dudvMap, distortedTexCoords).rg * 2.0f - 1.0f) * waveStrength * clamp(waterDepth / 20.0f, 0.0f, 1.0f);

	refractTexCoords += distorsion;
	reflectTexCoords += distorsion;

	// Avoid glitch at the bottom of the screen
	refractTexCoords = clamp(refractTexCoords, 0.001f, 0.999f);
	reflectTexCoords.x = clamp(reflectTexCoords.x, 0.001f, 0.999f);
	reflectTexCoords.y = clamp(reflectTexCoords.y, -0.999f, -0.001f);

	// Get the reflection and refraction colors from rendered textures
	vec4 reflectColor = texture(reflectionTexture, reflectTexCoords);
	vec4 refractColor = texture(refractionTexture, refractTexCoords);

	// Apply normal map
	vec4 normalMapColor = texture(normalMap, distortedTexCoords);
	vec3 normal = vec3(normalMapColor.r * 2.0f - 1.0f, normalMapColor.b * 3.0f, normalMapColor.g * 2.0f - 1.0f);
	normal = normalize(normal);

	// Apply fresnel
	vec3 viewVector = normalize(toCameraVector);
	float refractiveFactor = dot(viewVector, normal);
	refractiveFactor = pow(refractiveFactor, 1.0f);
	refractiveFactor = clamp(refractiveFactor, 0.0f, 1.0f);

	// Apply lights (Directional light)
	vec3 reflectedLight = reflect(normalize(fromLightVector), normal);
	float specular = max(dot(reflectedLight, viewVector), 0.0f);
	specular = pow(specular, shineDamper);
	vec3 specularHighlights = ligthColor * specular * reflectivity * clamp(waterDepth / 5.0f, 0.0f, 1.0f);

	// Get final color
	FragColor = mix(reflectColor, refractColor, refractiveFactor);
	FragColor = mix(FragColor, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.2f) + vec4(specularHighlights, 0.0f);
	FragColor.a = clamp(waterDepth / 2.0f, 0.0f, 1.0f);

}
