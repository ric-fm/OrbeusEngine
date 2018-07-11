#include "Engine.h"

#include "Window.h"
#include "World.h"
#include "RenderingEngine.h"
#include "Game.h"

#include "GLFW/glfw3.h"


void Engine::finish()
{
	if (game != nullptr)
	{
		delete game;
	}
	if (window != nullptr)
	{
		delete window;
	}
	if (input != nullptr)
	{
		delete input;
	}
	if (renderingEngine != nullptr)
	{
		delete renderingEngine;
	}

	glfwTerminate();
}

void Engine::run()
{
	init();

	isRunning = true;

	while (isRunning)
	{
		double currentFrame = glfwGetTime();
		deltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		update(deltaTime);

		render(deltaTime);
	}

	finish();
}

void Engine::init()
{
	window = new Window(800, 600, "Orbeus Engine");
	renderingEngine = new RenderingEngine(this);
	renderingEngine->init();

	Input::setWindow(window->getHandler());

	game->init();

	World::getInstance().init();
}

void Engine::update(float deltaTime)
{
	Input::update(deltaTime);

	World::getInstance().update(deltaTime);

	game->update(deltaTime);
}

void Engine::render(float deltaTime)
{
	renderingEngine->render(deltaTime);
}

void Engine::quitGame()
{
	isRunning = false;
}
