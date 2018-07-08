#include "RenderingEngine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Engine.h"
#include "Window.h"
#include "World.h"
#include "Log.h"

RenderingEngine::RenderingEngine(Engine* engine)
	: engine(engine)
{
}

void RenderingEngine::init()
{
	if (glewInit() == GLEW_OK)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		Log::error("Failed to init GLEW");
	}
}

void RenderingEngine::render(float deltaTime)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	World::getInstance().render(deltaTime, shader);

	glfwSwapBuffers(engine->getWindow()->getHandler());
}
