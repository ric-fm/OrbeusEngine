#pragma once

#include "Vector2.h"

struct GLFWwindow;



class Input
{
public:
	struct MouseState
	{
		Vector2 position;
		Vector2 delta;
	};

private:
	//static GLFWwindow* windowHandler;

	//static MouseState currentMouseState;
	//static MouseState lastMouseState;

public:
	static void setWindow(GLFWwindow* window);

	static void update(float deltaTime);

	static bool isKeyDown(int key);

	static bool isMouseButtonDown(int button);

	static Vector2 getMousePosition();

	static MouseState getMouseState();
};
