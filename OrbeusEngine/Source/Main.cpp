#include <iostream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"

#include "Mesh.h"

#include "Camera.h"
#include "Transform.h"
#include "World.h"

#include "GameObject.h"
#include "GameComponent.h"
#include "CameraController.h"
#include "Input.h"


GameObject* cameraGO;

float deltaTime = 0.0f;
double lastFrame = 0.0f;

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(800, 600, "Orbeus Engine", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Input* input = new Input(window);

	if (glewInit() != GLEW_OK)
	{
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	std::cout << glGetString(GL_VERSION) << std::endl;

	Shader* shader = new Shader("Resources/Shaders/Mesh-vs.shader", "Resources/Shaders/Mesh-fs.shader");

	Camera* aCamera = World::getInstance().getActiveCamera();

	cameraGO = World::getInstance().getActiveCamera()->getOwner();

	cameraGO->getTransform()->setPosition(Vector3(0.0f, 0.0f, 10.0f));
	cameraGO->getTransform()->setRotation(Vector3(0.0f, -90.0f, 0.0f));

	cameraGO->addComponent<CameraController>(new CameraController());

	GameObject g0("g0");
	g0.addComponent<Mesh>(new Mesh("Resources/Models/Cube/Cube.obj"));

	float sc = 0.8f;
	g0.getTransform()->setPosition(Vector3(0.0f, 2.0f, 0.0f));
	g0.getTransform()->setScale(Vector3(sc, sc, sc));
	g0.getTransform()->setRotation(Vector3(45.0f, 0.0f, 0.0f));


	GameComponent* goGet = g0.getComponent<GameComponent>();

	World::getInstance().addGameObject(&g0);

	World::getInstance().init();

	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		processInput(window);
		input->update(deltaTime);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		World::getInstance().update(deltaTime, input);

		World::getInstance().render(deltaTime, shader);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
