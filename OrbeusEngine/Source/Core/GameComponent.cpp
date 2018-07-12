#include "GameComponent.h"

#include "Transform.h"
#include "GameObject.h"

GameComponent::GameComponent()
{
}

GameComponent::~GameComponent()
{
}

Transform* GameComponent::getTransform() const
{ 
	return owner != nullptr ? owner->getTransform() : nullptr; 
}
