#pragma once

#include "Core/GameComponent.h"
#include "Core/Transform.h"

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
			getTransform()->setRelativePosition(target->getRelativePosition());
			getTransform()->setRelativeScale(target->getRelativeScale());
			getTransform()->setRelativeRotation(target->getRelativeRotation());
		}
	}
};
