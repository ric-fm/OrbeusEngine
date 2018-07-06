#pragma once

#include "Vector2.h"

struct GLFWwindow;

struct MouseState
{
	Vector2 position;
	Vector2 delta;
};

class Input
{
private:
	GLFWwindow * window;

	MouseState currentMouseState;
	MouseState lastMouseState;

public:
	Input(GLFWwindow* window);

	void update(float deltaTime);

	bool isKeyDown(int key);

	bool isMouseButtonDown(int button);

	Vector2 getMousePosition();

	const MouseState& getMouseState() const { return currentMouseState; }
};
