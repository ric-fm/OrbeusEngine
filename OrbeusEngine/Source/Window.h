#pragma once

struct GLFWwindow;


class Window
{
private:
	GLFWwindow* window;

public:
	Window(int width, int height, const char* title);

	GLFWwindow* getHandler() { return window; }

	void setCursorMode(unsigned int mode);
};