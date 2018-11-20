#pragma once

#include "Core/GameComponent.h"
#include "Math/Matrix4.h"
#include "Math/Vector3.h"


class Camera : public GameComponent
{
private:
	Matrix4 projection;

	float near;
	float far;
	float aspectRatio;

	float fov;

	Vector3 skyColor;
	Vector3 ambientLight;
	Vector3 fogColor;
	float fogDensity;

	void calculateProjectionMatrix();
public:
	Camera();
	Camera(float fov, float aspectRatio, float near, float far, Vector3 skyColor = Vector3(0.0f, 0.0f, 0.0f), Vector3 ambientLight = Vector3(0.1f, 0.1f, 0.1f), Vector3 fogColor = Vector3(0.2f, 0.2f, 0.2f), float fogDensity = 0.0f);
	~Camera();

	Vector3& getSkyColor() { return skyColor; }
	void setSkyColor(const Vector3& skyColor) { this->skyColor = skyColor; }

	Vector3& getAmbientLight() { return ambientLight; }
	void setAmbienLight(const Vector3& ambientLight) { this->ambientLight = ambientLight; }

	Vector3& getFogColor() { return fogColor; }
	void setFogColor(const Vector3& fogColor) { this->fogColor = fogColor; }

	float getFogDensity() const { return fogDensity; }
	void setFogDensity(float fogDensity) { this->fogDensity = fogDensity; }

	Matrix4 getViewMatrix() const;
	Matrix4 getProjectionMatrix() const { return projection; }
};