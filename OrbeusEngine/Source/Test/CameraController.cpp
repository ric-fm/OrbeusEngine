#include "CameraController.h"

#include "GLFW/glfw3.h"

#include "Core/World.h"
#include "Core/Transform.h"
#include "Math/Math.h"

#include "Core/Engine.h"
#include "Window/Window.h"

void CameraController::update(float deltaTime)
{
	bool rightButton = Input::isMouseButtonDown(GLFW_MOUSE_BUTTON_RIGHT);
	if (rightButton != lastRightButton)
	{
		if (rightButton)
			Engine::getInstance().getWindow()->setCursorMode(GLFW_CURSOR_DISABLED);
		else
			Engine::getInstance().getWindow()->setCursorMode(GLFW_CURSOR_NORMAL);
	}
	if (rightButton)
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
			getTransform()->addPosition(-Vector3::up * speed);
		}
		if (Input::isKeyDown(GLFW_KEY_E))
		{
			getTransform()->addPosition(Vector3::up * speed);
		}

		float scroll = Input::getMouseState().scroll;
		cameraSpeed += scroll * incrementSpeed;
		cameraSpeed = Math::clamp(cameraSpeed, minCameraSpeed, maxCameraSpeed);

		Vector2 delta = Input::getMouseState().delta;

		delta *= mouseSensitivity;

		yaw += (float)delta.x;
		pitch -= (float)delta.y;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(pitch, yaw, 0.0f)));
	}
	lastRightButton = rightButton;
}
