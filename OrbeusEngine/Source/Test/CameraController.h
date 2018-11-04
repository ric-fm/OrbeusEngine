#pragma once

#include "Core/GameComponent.h"

class CameraController : public GameComponent
{
private:
	float yaw = 0;
	float pitch = 0;

public:
	float cameraSpeed = 15.0f;
	float mouseSensitivity = 0.1f;
	virtual void update(float deltaTime) override;

};
