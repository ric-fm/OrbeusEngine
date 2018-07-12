#pragma once

#include "Vector3.h"
#include "Texture.h"

struct Material
{
	Vector3 diffuse;
	Vector3 specular;

	Texture* diffuseTexture;
	Texture* specularTexture;

	bool useDiffuseTexture;
	bool useSpecularTexture;

	float specularIntensity;
	float specularPower;

	Material(const Vector3& diffuse, const Vector3& specular) 
		: diffuse(diffuse), specular(specular),
		diffuseTexture(nullptr), specularTexture(nullptr),
		useDiffuseTexture(false), useSpecularTexture(false),
		specularIntensity(1.0f), specularPower(32.0f)
	{
	}
};
