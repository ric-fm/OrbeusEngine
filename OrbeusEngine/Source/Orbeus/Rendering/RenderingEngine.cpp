#include "RenderingEngine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Orbeus/Window/Window.h"
#include "Orbeus/Math/Vector3.h"
#include "Orbeus/Core/Engine.h"
#include "Orbeus/Core/World.h"
#include "Orbeus/Components/Camera.h"

#include "Orbeus/Rendering/Mesh/MeshRenderer.h"
#include "Orbeus/Rendering/SkeletalMesh/SkeletalMeshRenderer.h"
#include "Orbeus/Rendering/Text/TextRenderer.h"
#include "Orbeus/Rendering/Terrain/TerrainRenderer.h"
#include "Orbeus/Rendering/SkyBoxRenderer.h"
#include "Orbeus/Rendering/Water/WaterRenderer.h"
#include "Orbeus/Logging/VisualLogger.h"

#include "Orbeus/Utils/Log.h"

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
	if (skeletalMeshRenderer != nullptr)
	{
		delete skeletalMeshRenderer;
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
	if (waterRenderer != nullptr)
	{
		delete waterRenderer;
	}
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
	skeletalMeshRenderer = new SkeletalMeshRenderer();
	textRenderer = new TextRenderer();
	terrainRenderer = new TerrainRenderer();
	skyBoxRenderer = new SkyBoxRenderer();
	waterRenderer = new WaterRenderer();
}

void RenderingEngine::renderScene(Camera* camera)
{
	Vector3 clearColor = camera->getSkyColor();
	glClearColor(clearColor.x, clearColor.y, clearColor.z, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (camera->getIsClipPlaneEnabled())
	{
		glEnable(GL_CLIP_DISTANCE0);
	}

	meshRenderer->render(camera);
	skeletalMeshRenderer->render(camera);
	terrainRenderer->render(camera);
	skyBoxRenderer->render(camera);

	if (camera->getIsClipPlaneEnabled())
	{
		glDisable(GL_CLIP_DISTANCE0);
	}
}

void RenderingEngine::render(float deltaTime)
{
	renderScene(World::getInstance().getActiveCamera());
	waterRenderer->render(deltaTime);
	textRenderer->render();

	// Log Rendering
	VisualLogger::getInstance().render();
 
	glfwSwapBuffers(engine->getWindow()->getHandler());
}

void RenderingEngine::setBlendingEnabled(bool enabled)
{
	if (enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}
}
