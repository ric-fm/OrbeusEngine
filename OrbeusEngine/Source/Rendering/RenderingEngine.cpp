#include "RenderingEngine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Window/Window.h"
#include "Core/Engine.h"
#include "Core/World.h"
#include "Core/Transform.h"
#include "Components/Camera.h"
#include "Shader.h"
#include "Utils/Log.h"

#include "Rendering/Mesh/MeshRenderer.h"
#include "Rendering/Text/TextRenderer.h"
#include "Rendering/Terrain/TerrainRenderer.h"
#include "Rendering/SkyBoxRenderer.h"
#include "Logging/VisualLogger.h"

RenderingEngine::RenderingEngine(Engine* engine)
	: engine(engine)
{
}

RenderingEngine::~RenderingEngine()
{
	if (meshRenderer != nullptr)
	{
		delete meshRenderer;
	}
	if (textRenderer != nullptr)
	{
		delete textRenderer;
	}
	if (terrainRenderer != nullptr)
	{
		delete terrainRenderer;
	}
	if (skyBoxRenderer != nullptr)
	{
		delete skyBoxRenderer;
	}
}

void RenderingEngine::registerLight(Light* light)
{
	lights.push_back(light);
}

void RenderingEngine::init()
{
	if (glewInit() == GLEW_OK)
	{
		Log::info((const char*)glGetString(GL_VERSION));
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_CLAMP);
		glFrontFace(GL_CW);
	}
	else
	{
		Log::error("Failed to init GLEW");
	}

	meshRenderer = new MeshRenderer();
	textRenderer = new TextRenderer();
	terrainRenderer = new TerrainRenderer();
	skyBoxRenderer = new SkyBoxRenderer();
}

void RenderingEngine::render(float deltaTime)
{
	Vector3 skyColor = World::getInstance().getActiveCamera()->getSkyColor();
	glClearColor(skyColor.x, skyColor.y, skyColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	meshRenderer->render();
	terrainRenderer->render();
	skyBoxRenderer->render();
	textRenderer->render();

	// Log Rendering
	VisualLogger::getInstance().render();
 
	glfwSwapBuffers(engine->getWindow()->getHandler());
}
