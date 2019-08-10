#pragma once

#include "Orbeus/Core/Engine.h"
#include "Orbeus/Core/Input.h"
#include "Orbeus/Core/World.h"
#include "Orbeus/Core/Transform.h"
#include "Orbeus/Core/GameObject.h"
#include "Orbeus/Core/GameComponent.h"
#include "Orbeus/Components/Camera.h"
#include "Orbeus/Rendering/RenderingEngine.h"

class Game
{
public:

	virtual void preInit() {}
	virtual void postInit() {}
	virtual void update(float deltaTime) {}
};