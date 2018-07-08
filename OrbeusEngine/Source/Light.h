#pragma once

#include "Vector3.h"

struct BaseLight
{
	Vector3 color;
	float intensity;

	BaseLight(const BaseLight& baseLight)
		: color(baseLight.color), intensity(baseLight.intensity)
	{
	}

	BaseLight(const Vector3& color, float intensity)
		: color(color), intensity(intensity)
	{
	}
};

struct DirectionalLight
{
	BaseLight base;
	Vector3 direction;

	DirectionalLight(const DirectionalLight& directionalLight)
		: base(directionalLight.base), direction(directionalLight.direction)
	{
	}

	DirectionalLight(const BaseLight& base, Vector3 direction)
		: base(base.color, base.intensity), direction(direction)
	{
	}

	DirectionalLight(const Vector3 color, float intensity, Vector3 direction)
		: base(color, intensity), direction(direction)
	{
	}
};
