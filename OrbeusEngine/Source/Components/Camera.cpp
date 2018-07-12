#include "Camera.h"

#include "Core/Transform.h"

Camera::Camera()
	: fov(90.0f), aspectRatio(800.0f/600.0f), near(0.1f), far(100.0f)
{
	calculateProjectionMatrix();
}

Camera::Camera(float fov, float aspectRatio, float near, float far)
	: fov(fov), aspectRatio(aspectRatio), near(near), far(far)
{
	calculateProjectionMatrix();
}

Camera::~Camera()
{
}

void Camera::calculateProjectionMatrix()
{
	projection = Matrix4::Perspective(fov, aspectRatio, near, far);
}


Matrix4 Camera::getViewMatrix() const
{
	return Matrix4::LookAt(getTransform()->getPosition(), getTransform()->getPosition() + getTransform()->getForwardVector(), getTransform()->getUpVector());
}
