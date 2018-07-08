#pragma once

#include "GameComponent.h"
#include "GameObject.h"
#include "Transform.h"

class Rotate : public GameComponent
{
private:
	Vector3 rotateSpeed;
public:
	Rotate(const Vector3& rotateSpeed)
		: rotateSpeed(rotateSpeed)
	{

	}

	void setRotateSpeed(const Vector3& rotateSpeed) { this->rotateSpeed = rotateSpeed; }

	virtual void update(float deltaTime) override
	{

		Vector3 currentRotation = getTransform()->getRotation();


		getTransform()->setRotation(currentRotation + rotateSpeed * deltaTime);
	}
};