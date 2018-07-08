#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Log.h"

Window::Window(int width, int height, const char* title)
{
	if (glfwInit())
	{
		window = glfwCreateWindow(width, height, title, NULL, NULL);
		if (window)
		{
			glfwMakeContextCurrent(window);
		}
		else
		{
			Log::error("Failed creating window");
			glfwTerminate();
		}

	}
	else
	{
		Log::error("Failed to init GLFW");
	}
}

void Window::setCursorMode(unsigned int mode)
{
	glfwSetInputMode(window, GLFW_CURSOR, mode);
}
