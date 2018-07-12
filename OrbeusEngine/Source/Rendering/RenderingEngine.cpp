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

RenderingEngine::RenderingEngine(Engine* engine)
	: engine(engine), ambientLight(0.0f, 0.0f, 0.0f)
{
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
	}
	else
	{
		Log::error("Failed to init GLEW");
	}

	ambientShader = new Shader("Resources/Shaders/Forward/Ambient-vs.shader", "Resources/Shaders/Forward/Ambient-fs.shader");
}

void RenderingEngine::render(float deltaTime)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ambientShader->bind();
	ambientShader->SetFloat3("ambientLight", ambientLight);

	World::getInstance().render(deltaTime, ambientShader);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDepthMask(false);
	glDepthFunc(GL_EQUAL);

	for (int i = 0; i < lights.size(); ++i)
	{
		lights[i]->updateShader();

		World::getInstance().render(deltaTime, lights[i]->getShader());
	}

	glDepthFunc(GL_LESS);
	glDepthMask(true);
	glDisable(GL_BLEND);

	glfwSwapBuffers(engine->getWindow()->getHandler());
}
