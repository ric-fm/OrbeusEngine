#pragma once

#include "GameComponent.h"
#include "Vector3.h"
#include "Transform.h"

class CopyTransform : public GameComponent
{
private:
	Transform* target;
public:
	CopyTransform(Transform* target) : target(target) { }
	
	void setTarget(Transform* target) { this->target = target; }
	Transform* getTarget() const { return target; }

	virtual void update(float deltaTime)
	{
		if (target != nullptr)
		{
			getTransform()->setPosition(target->getPosition());
			getTransform()->setRotation(target->getRotation());
			getTransform()->setScale(target->getScale());
		}
	}
};
