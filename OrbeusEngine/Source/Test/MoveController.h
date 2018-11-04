#pragma once

#include "Core/GameComponent.h"

class MoveController : public GameComponent
{
private:

public:
	float speed = 15.0f;
	float turnSpeed = 100.0f;

	float yaw = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;

	bool free = true;

	virtual void init() override;
	virtual void update(float deltaTime) override;

};
