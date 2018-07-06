#include "CameraController.h"

#include <iostream>


#include "GLFW/glfw3.h"
#include "Transform.h"
#include "World.h"

#include "Input.h"



void CameraController::update(float deltaTime, Input* input)
{
	float speed = cameraSpeed * deltaTime;

	if (input->isKeyDown(GLFW_KEY_W))
	{
		getTransform()->addPosition(getTransform()->getForwardVector() * speed);
	}
	if (input->isKeyDown(GLFW_KEY_S))
	{
		getTransform()->addPosition(-getTransform()->getForwardVector() * speed);
	}
	if (input->isKeyDown(GLFW_KEY_A))
	{
		getTransform()->addPosition(-getTransform()->getRightVector() * speed);
	}
	if (input->isKeyDown(GLFW_KEY_D))
	{
		getTransform()->addPosition(getTransform()->getRightVector() * speed);
	}

	if (input->isKeyDown(GLFW_KEY_Q))
	{
		getTransform()->addPosition(-World::getUpVector() * speed);
	}
	if (input->isKeyDown(GLFW_KEY_E))
	{
		getTransform()->addPosition(World::getUpVector() * speed);
	}


	Vector2 delta = input->getMouseState().delta;

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
