#include "Input.h"

#include "GLFW/glfw3.h"


static GLFWwindow* windowHandler;

static Input::MouseState currentMouseState;
static Input::MouseState lastMouseState;

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	currentMouseState.scroll = (float)yoffset;
}

void Input::setWindow(GLFWwindow* window)
{
	windowHandler = window;
	lastMouseState.position = getMousePosition();

	glfwSetScrollCallback(windowHandler, scroll_callback);
}

void Input::update(float deltaTime)
{
	currentMouseState.position = getMousePosition();
	currentMouseState.delta.x = (currentMouseState.position.x - lastMouseState.position.x);
	currentMouseState.delta.y = (lastMouseState.position.y - currentMouseState.position.y);

	lastMouseState.delta = currentMouseState.delta;
	lastMouseState.position = currentMouseState.position;
	lastMouseState.scroll = currentMouseState.scroll;

	currentMouseState.scroll = 0.0f;

	glfwPollEvents();
}

bool Input::isKeyDown(int key)
{
	return glfwGetKey(windowHandler, key) == GLFW_PRESS;
}

bool Input::isMouseButtonDown(int button)
{
	return glfwGetMouseButton(windowHandler, button) == GLFW_PRESS;
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
