#include "Input.h"

#include "GLFW/glfw3.h"


static GLFWwindow* windowHandler;

static Input::MouseState currentMouseState;
static Input::MouseState lastMouseState;

void Input::setWindow(GLFWwindow* window)
{
	windowHandler = window;
	lastMouseState.position = getMousePosition();
}

void Input::update(float deltaTime)
{
	currentMouseState.position = getMousePosition();
	currentMouseState.delta.x = (currentMouseState.position.x - lastMouseState.position.x);
	currentMouseState.delta.y = (lastMouseState.position.y - currentMouseState.position.y);

	lastMouseState.delta = currentMouseState.delta;
	lastMouseState.position = currentMouseState.position;

	glfwPollEvents();
}

bool Input::isKeyDown(int key)
{
	return glfwGetKey(windowHandler, key) == GLFW_PRESS;
}

bool Input::isMouseButtonDown(int button)
{
	return glfwGetMouseButton(windowHandler, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

Vector2 Input::getMousePosition()
{
	double x, y;
	glfwGetCursorPos(windowHandler, &x, &y);

	return Vector2((float)x, (float)y);
}

Input::MouseState Input::getMouseState()
{
	return currentMouseState;
}
