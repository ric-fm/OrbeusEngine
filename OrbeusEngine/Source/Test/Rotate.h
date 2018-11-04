#pragma once

#include "Core/GameComponent.h"
#include "Core/Transform.h"

class Rotate : public GameComponent
{
private:
	Vector3 axis;
	float speed;
	float angle;
public:
	Rotate(const Vector3& axis, float speed)
		: axis(axis), speed(speed), angle(0.0f)
	{
	}

	virtual void update(float deltaTime) override
	{
		angle += speed * deltaTime;
		getTransform()->setRelativeRotation(Quaternion::AxisAngle(axis, angle));
	}
};