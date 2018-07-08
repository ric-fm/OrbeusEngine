#include "RenderingEngine.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Engine.h"
#include "Window.h"
#include "World.h"
#include "Log.h"
#include "Shader.h"
#include "Camera.h"
#include "Transform.h"

RenderingEngine::RenderingEngine(Engine* engine)
	: engine(engine), ambientLight(0.0f, 0.0f, 0.0f), directionalLight(Vector3(1.0f, 1.0f, 1.0f), 1.0f, Vector3())
{
}

void RenderingEngine::init()
{
	if (glewInit() == GLEW_OK)
	{
		Log::info((const char*)glGetString(GL_VERSION));
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		Log::error("Failed to init GLEW");
	}

	setAmbienLight(Vector3(0.1f, 0.1f, 0.1f));
	setDirectionalLight(DirectionalLight(Vector3(1.0f, 1.0f, 1.0f), 0.8f, Vector3(0.5f, -1.0f, -0.6f)));
}

void RenderingEngine::render(float deltaTime)
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->bind();

	shader->SetFloat3("ambientLight", ambientLight);

	shader->SetFloat3("directionalLight.base.color", directionalLight.base.color);
	shader->SetFloat("directionalLight.base.intensity", directionalLight.base.intensity);
	shader->SetFloat3("directionalLight.direction", directionalLight.direction);

	shader->SetFloat("material.specularIntensity", 1);
	shader->SetFloat("material.specularPower", 32);
	shader->SetFloat3("viewPos", World::getInstance().getActiveCamera()->getTransform()->getPosition());


	World::getInstance().render(deltaTime, shader);

	glfwSwapBuffers(engine->getWindow()->getHandler());
}
