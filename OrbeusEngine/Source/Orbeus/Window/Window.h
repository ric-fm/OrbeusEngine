#pragma once

struct GLFWwindow;


class Window
{
private:
	GLFWwindow* window;

	int width;
	int height;

public:
	Window(int width, int height, const char* title);

	GLFWwindow* getHandler() { return window; }

	void setCursorMode(unsigned int mode);

	void resize(int newWidth, int newHeight);

	unsigned int getWidth() const { return width; }
	unsigned int getHeight() const { return height; }
};