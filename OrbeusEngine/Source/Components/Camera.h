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

	Vector3 ambientLight;

	void calculateProjectionMatrix();
public:
	Camera();
	Camera(float fov, float aspectRatio, float near, float far);
	~Camera();

	Vector3& getAmbientLight() { return ambientLight; }
	void setAmbienLight(const Vector3& ambientLight) { this->ambientLight = ambientLight; }

	Matrix4 getViewMatrix() const;
	Matrix4 getProjectionMatrix() const { return projection; }
};