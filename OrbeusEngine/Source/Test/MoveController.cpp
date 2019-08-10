#include "MoveController.h"

#include "GLFW/glfw3.h"

#include "Orbeus/Core/World.h"
#include "Orbeus/Core/Transform.h"

#include "Orbeus/Utils/Log.h"

void MoveController::init()
{
	pitch = getTransform()->getRelativeRotation().x;
	yaw = getTransform()->getRelativeRotation().y;
	roll = getTransform()->getRelativeRotation().z;
}

void MoveController::update(float deltaTime)
{
	float movement = speed * deltaTime;
	if (Input::isKeyDown(GLFW_KEY_UP))
	{
		getTransform()->addPosition(getTransform()->getForwardVector() * movement);
	}
	if (Input::isKeyDown(GLFW_KEY_DOWN))
	{
		getTransform()->addPosition(-getTransform()->getForwardVector() * movement);
	}
	if (Input::isKeyDown(GLFW_KEY_LEFT))
	{
		getTransform()->addPosition(-getTransform()->getRightVector() * movement);
	}
	if (Input::isKeyDown(GLFW_KEY_RIGHT))
	{
		getTransform()->addPosition(getTransform()->getRightVector() * movement);
	}

	if (Input::isKeyDown(GLFW_KEY_PAGE_DOWN))
	{
		getTransform()->addPosition(-Vector3::up * movement);
	}
	if (Input::isKeyDown(GLFW_KEY_PAGE_UP))
	{
		getTransform()->addPosition(Vector3::up * movement);
	}

	if (free)
	{
		if (Input::isKeyDown(GLFW_KEY_J))
		{
			yaw -= turnSpeed * deltaTime;
		}
		if (Input::isKeyDown(GLFW_KEY_L))
		{
			yaw += turnSpeed * deltaTime;
		}

		if (Input::isKeyDown(GLFW_KEY_I))
		{
			pitch -= turnSpeed * deltaTime;
		}
		if (Input::isKeyDown(GLFW_KEY_K))
		{
			pitch += turnSpeed * deltaTime;
		}

		if (Input::isKeyDown(GLFW_KEY_U))
		{
			roll += turnSpeed * deltaTime;
		}
		if (Input::isKeyDown(GLFW_KEY_O))
		{
			roll -= turnSpeed * deltaTime;
		}
	}
	else
	{


		float inc = 45.0f;
		static bool previusDown[6] = { false, false, false, false, false, false };
		if (Input::isKeyDown(GLFW_KEY_J) && !previusDown[0])
		{
			yaw -= inc;
		}
		if (Input::isKeyDown(GLFW_KEY_L) && !previusDown[1])
		{
			yaw += inc;
		}

		if (Input::isKeyDown(GLFW_KEY_I) && !previusDown[2])
		{
			pitch -= inc;
		}
		if (Input::isKeyDown(GLFW_KEY_K) && !previusDown[3])
		{
			pitch += inc;
		}

		if (Input::isKeyDown(GLFW_KEY_U) && !previusDown[4])
		{
			roll += inc;
		}
		if (Input::isKeyDown(GLFW_KEY_O) && !previusDown[5])
		{
			roll -= inc;
		}

		previusDown[0] = Input::isKeyDown(GLFW_KEY_J);
		previusDown[1] = Input::isKeyDown(GLFW_KEY_L);
		previusDown[2] = Input::isKeyDown(GLFW_KEY_I);
		previusDown[3] = Input::isKeyDown(GLFW_KEY_K);
		previusDown[4] = Input::isKeyDown(GLFW_KEY_U);
		previusDown[5] = Input::isKeyDown(GLFW_KEY_O);
	}


	if (Input::isKeyDown(GLFW_KEY_P))
	{
		yaw = pitch = roll = 0.0f;
	}

	if (Input::isKeyDown(GLFW_KEY_N))
	{
		free = true;
	}
	if (Input::isKeyDown(GLFW_KEY_M))
	{
		free = false;
	}

	float x = pitch;
	float y = yaw;
	float z = roll;
	getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(x, y, z)));
}
