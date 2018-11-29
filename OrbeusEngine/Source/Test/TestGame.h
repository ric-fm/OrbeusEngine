#pragma once

#include "Core/Game.h"

class GameObject;

class TestGame : public Game
{
public:
	virtual void preInit() override;
	virtual void postInit() override;
	virtual void update(float deltaTime) override;

	class GUIText* guiText;

	GameObject* waterGO;
	GameObject* waterRefractionGO;
	GameObject* waterReflectionGO;
};