#pragma once

#include "Core/GameComponent.h"
#include "Math/Matrix4.h"

class Camera : public GameComponent
{
public:
private:
	Matrix4 projection;

	float near;
	float far;
	float aspectRatio;

	float fov;

	void calculateProjectionMatrix();
public:
	Camera();
	Camera(float fov, float aspectRatio, float near, float far);
	~Camera();

	Matrix4 getViewMatrix() const;
	Matrix4 getProjectionMatrix() const { return projection; }
};