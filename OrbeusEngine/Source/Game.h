#pragma once

#include "Input.h"
#include "World.h"
#include "Transform.h"
#include "GameObject.h"
#include "GameComponent.h"
#include "Camera.h"
#include "Mesh.h"
#include "Engine.h"
#include "RenderingEngine.h"

class Game
{
public:

	virtual void init() {}
	virtual void update(float deltaTime) {}
};