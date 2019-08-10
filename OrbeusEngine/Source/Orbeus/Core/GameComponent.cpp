#include "GameComponent.h"

#include "Orbeus/Core/Transform.h"
#include "Orbeus/Core/GameObject.h"

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
