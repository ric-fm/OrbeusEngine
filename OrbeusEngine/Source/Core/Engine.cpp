#include "Engine.h"

#include "Window/Window.h"
#include "World.h"
#include "Rendering/RenderingEngine.h"
#include "Game.h"
#include "Editor/EditorManager.h"
#include "Editor/AnimatedModelEditor.h"

#include "GLFW/glfw3.h"

#include "Logging/VisualLogger.h"


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

	game->preInit();

	World::getInstance().init();
	VisualLogger::getInstance().init();

	game->postInit();

	EditorManager::getInstance().addEditor(new AnimatedModelEditor());
	EditorManager::getInstance().init();
}

void Engine::update(float deltaTime)
{
	Input::update(deltaTime);

	World::getInstance().update(deltaTime);
	VisualLogger::getInstance().update(deltaTime);
	EditorManager::getInstance().update(deltaTime);

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
