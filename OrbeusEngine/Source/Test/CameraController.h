#pragma once

#include "Orbeus/Core/GameComponent.h"

class CameraController : public GameComponent
{
private:
	float yaw = 0;
	float pitch = 0;

	bool lastRightButton = false;

public:
	float cameraSpeed = 15.0f;
	float mouseSensitivity = 0.1f;
	float minCameraSpeed = 10.0f;
	float maxCameraSpeed = 500.0f;
	float incrementSpeed = 10.0f;
	virtual void update(float deltaTime) override;

};
