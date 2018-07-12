#include "CameraController.h"

#include "GLFW/glfw3.h"

#include "Core/World.h"
#include "Core/Transform.h"

void CameraController::update(float deltaTime)
{
	float speed = cameraSpeed * deltaTime;

	if (Input::isKeyDown(GLFW_KEY_W))
	{
		getTransform()->addPosition(getTransform()->getForwardVector() * speed);
	}
	if (Input::isKeyDown(GLFW_KEY_S))
	{
		getTransform()->addPosition(-getTransform()->getForwardVector() * speed);
	}
	if (Input::isKeyDown(GLFW_KEY_A))
	{
		getTransform()->addPosition(-getTransform()->getRightVector() * speed);
	}
	if (Input::isKeyDown(GLFW_KEY_D))
	{
		getTransform()->addPosition(getTransform()->getRightVector() * speed);
	}

	if (Input::isKeyDown(GLFW_KEY_Q))
	{
		getTransform()->addPosition(-World::getUpVector() * speed);
	}
	if (Input::isKeyDown(GLFW_KEY_E))
	{
		getTransform()->addPosition(World::getUpVector() * speed);
	}

	Vector2 delta = Input::getMouseState().delta;

	delta *= mouseSensitivity;

	float yaw = getTransform()->getRotation().y;
	float pitch = getTransform()->getRotation().x;

	yaw += (float)delta.x;
	pitch += (float)delta.y;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	getTransform()->setRotation(Vector3(pitch, yaw, getTransform()->getRotation().z));
}
