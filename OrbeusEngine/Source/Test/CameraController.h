#pragma once

#include "Core/GameComponent.h"

class CameraController : public GameComponent
{
private:

public:
	float cameraSpeed = 15.0f;
	float mouseSensitivity = 0.1f;
	virtual void update(float deltaTime) override;

};
