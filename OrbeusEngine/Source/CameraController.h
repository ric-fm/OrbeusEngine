#pragma once

#include "GameComponent.h"

class CameraController : public GameComponent
{
private:

public:
	float cameraSpeed = 50.0f;
	float mouseSensitivity = 0.1f;
	virtual void update(float deltaTime, Input* input) override;

};
