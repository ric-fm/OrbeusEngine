#pragma once

#include "Core/Game.h"

class TestGame : public Game
{
public:
	virtual void init() override;
	virtual void update(float deltaTime) override;

	class GUIText* guiText;
};