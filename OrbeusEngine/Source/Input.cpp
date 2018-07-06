#include "Input.h"

#include "GLFW/glfw3.h"


Input::Input(GLFWwindow* window)
	: window(window)
{
	lastMouseState.position = getMousePosition();
}

void Input::update(float deltaTime)
{
	currentMouseState.position = getMousePosition();
	currentMouseState.delta.x = (currentMouseState.position.x - lastMouseState.position.x);
	currentMouseState.delta.y = (lastMouseState.position.y - currentMouseState.position.y);

	lastMouseState.delta = currentMouseState.delta;
	lastMouseState.position = currentMouseState.position;
}

bool Input::isKeyDown(int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

bool Input::isMouseButtonDown(int button)
{
	return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

Vector2 Input::getMousePosition()
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	return Vector2((float)x, (float)y);
}
