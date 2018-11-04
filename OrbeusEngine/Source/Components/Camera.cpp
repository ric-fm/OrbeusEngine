#include "Camera.h"

#include "Core/Transform.h"
#include "Core/GameObject.h"

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
	// Get the inverted camera transform matrix
	Matrix4 view = getOwner()->getTransform()->getMatrix().getInversed();

	// Ivert OpenGL Camera forward (because in OpenGL the forward vector is -z)
	view.buffer[0][2] *= -1.0f;
	view.buffer[1][2] *= -1.0f;
	view.buffer[2][2] *= -1.0f;
	view.buffer[3][2] *= -1.0f;

	return view;
}
