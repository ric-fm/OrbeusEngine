#pragma once

#include "Engine.h"
#include "Input.h"
#include "World.h"
#include "Transform.h"
#include "GameObject.h"
#include "GameComponent.h"
#include "Components/Camera.h"
#include "Rendering/RenderingEngine.h"

class Game
{
public:

	virtual void preInit() {}
	virtual void postInit() {}
	virtual void update(float deltaTime) {}
};