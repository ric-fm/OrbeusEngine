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

	DirectionalLight(const BaseLight& base, const Vector3& direction)
		: base(base.color, base.intensity), direction(direction)
	{
	}

	DirectionalLight(const Vector3& color, float intensity, const Vector3& direction)
		: base(color, intensity), direction(direction)
	{
	}
};

struct Attenuation
{
	float constant;
	float linear;
	float exponential;

	Attenuation(const Attenuation& attenuation)
		: constant(attenuation.constant), linear(attenuation.linear), exponential(attenuation.exponential)
	{
	}

	Attenuation(float constant, float linear, float exponential)
		: constant(constant), linear(linear), exponential(exponential)
	{
	}
};

#define MAX_POINT_LIGHTS 4

struct PointLight
{
	BaseLight base;
	Vector3 position;

	Attenuation attenuation;
	float radius;

	PointLight(const PointLight& pointLight)
		: base(pointLight.base), position(pointLight.position), attenuation(pointLight.attenuation), radius(pointLight.radius)
	{
	}

	PointLight(const BaseLight& base, Vector3 position, const Attenuation& attenuation, float radius)
		: base(base.color, base.intensity), position(position), attenuation(attenuation), radius(radius)
	{
	}

	PointLight(const Vector3& color, float intensity, const Vector3& position, float constant, float linear, float exponential, float radius)
		: base(color, intensity), position(position), attenuation(constant, linear, exponential), radius(radius)
	{
	}
};

#define MAX_SPOT_LIGHTS 4

struct SpotLight
{
	PointLight pointLight;
	Vector3 direction;
	float cutoff;

	SpotLight(const SpotLight& spotLight)
		: pointLight(spotLight.pointLight), direction(spotLight.direction), cutoff(spotLight.cutoff)
	{
	}

	SpotLight(const PointLight& pointLight, Vector3 position, const Vector3& direction, float cutoff)
		: pointLight(pointLight), direction(direction.normalize()), cutoff(cutoff)
	{
	}

	SpotLight(const Vector3& color, float intensity, const Vector3& position, float constant, float linear, float exponential, float radius, const Vector3& direction, float cutoff)
		: pointLight(color, intensity, position, constant, linear, exponential, radius), direction(direction.normalize()), cutoff(cutoff)
	{
	}
};
