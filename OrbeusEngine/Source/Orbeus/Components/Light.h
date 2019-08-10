#pragma once

#include "Orbeus/Core/GameComponent.h"

#include "Orbeus/Math/Vector3.h"

class Light : public GameComponent
{
protected:
	Vector3 color;
	float intensity;

public:
	Light(const Light& baseLight)
		: color(baseLight.color), intensity(baseLight.intensity)
	{
	}

	Light(const Vector3& color, float intensity)
		: color(color), intensity(intensity)
	{
	}

	void setColor(const Vector3& color) { this->color = color; }
	void setColor(float r, float g, float b)
	{
		this->color.x = r;
		this->color.y = g;
		this->color.z = b;
	}
	const Vector3& getColor() const { return color; }

	void setIntensity(float intensity) { this->intensity = intensity; }
	float getIntensity() const { return intensity; }

};

class DirectionalLight : public Light
{

public:
	DirectionalLight(const DirectionalLight& directionalLight)
		: Light(directionalLight.color, directionalLight.intensity)
	{
	}

	DirectionalLight(const Vector3& color, float intensity)
		: Light(color, intensity)
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

class PointLight : public Light
{
protected:
	Attenuation attenuation;
	float radius;

public:
	PointLight(const PointLight& pointLight)
		: PointLight(pointLight.color, pointLight.intensity,
			pointLight.attenuation.constant, pointLight.attenuation.linear, pointLight.attenuation.exponential,
			pointLight.radius)
	{
	}

	PointLight(const Vector3& color, float intensity, float constant, float linear, float exponential, float radius)
		: Light(color, intensity), attenuation(constant, linear, exponential), radius(radius)
	{
	}

	void setAttenuation(const Attenuation& attenuation) { this->attenuation = attenuation; }
	void setAttenuation(float constant, float linear, float exponential)
	{
		attenuation.constant = constant;
		attenuation.linear = linear;
		attenuation.exponential = exponential;
	}
	const Attenuation& getAttenuation() const { return attenuation; }

	void setRadius(float radius) { this->radius = radius; }
	float getRadius() const { return radius; }

};

class SpotLight : public PointLight
{
protected:
	float cutoff;

public:
	SpotLight(const SpotLight& spotLight)
		: SpotLight(spotLight.color, spotLight.intensity,
			spotLight.getAttenuation().constant, spotLight.getAttenuation().linear, spotLight.getAttenuation().exponential,
			spotLight.radius, spotLight.cutoff)
	{
	}

	SpotLight(const Vector3& color, float intensity, float constant, float linear, float exponential, float radius, float cutoff)
		: PointLight(color, intensity, constant, linear, exponential, radius), cutoff(cutoff)
	{
	}

	void setCutoff(float cutoff) { this->cutoff = cutoff; }
	float getCutOff() const { return cutoff; }

};
