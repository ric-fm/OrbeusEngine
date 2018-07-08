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
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_CLAMP);
	}
	else
	{
		Log::error("Failed to init GLEW");
	}

	setAmbienLight(Vector3(0.1f, 0.1f, 0.1f));
	setDirectionalLight(DirectionalLight(Vector3(1.0f, 1.0f, 1.0f), 0.8f, Vector3(0.5f, -1.0f, -0.6f)));

	PointLight pl0(Vector3(1.0f, 0.0f, 0.0f), 0.5f, Vector3(0.0f, 0.0f, 2.0f), 1.0f, 0.09f, 0.032f);
	PointLight pl1(Vector3(0.0f, 0.0f, 1.0f), 0.5f, Vector3(2.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f);

	pointLights.push_back(pl0);
	pointLights.push_back(pl1);
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

	for (int i = 0; i < MAX_POINT_LIGHTS && i < pointLights.size(); ++i)
	{
		PointLight& pointLight = pointLights[i];
		std::string preffix = "pointLights[" + std::to_string(i) + "]";

		shader->SetFloat3(preffix + ".base.color", pointLight.base.color);
		shader->SetFloat(preffix +  ".base.intensity", pointLight.base.intensity);
		shader->SetFloat3(preffix + ".position", pointLight.position);
		shader->SetFloat(preffix + ".attenuation.constant", pointLight.attenuation.constant);
		shader->SetFloat(preffix + ".attenuation.linear", pointLight.attenuation.linear);
		shader->SetFloat(preffix + ".attenuation.exponential", pointLight.attenuation.exponential);
	}

	shader->SetFloat("material.specularIntensity", 1);
	shader->SetFloat("material.specularPower", 32);
	shader->SetFloat3("viewPos", World::getInstance().getActiveCamera()->getTransform()->getPosition());


	World::getInstance().render(deltaTime, shader);

	glfwSwapBuffers(engine->getWindow()->getHandler());
}
