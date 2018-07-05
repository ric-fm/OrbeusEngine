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

GameObject* cameraGO;
float cameraSpeed = 50.0f;
float deltaTime = 0.0f;
double lastFrame = 0.0f;

double lastX = 400, lastY = 300;

bool firstMouse = true;

void processInput(GLFWwindow *Window)
{
	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(Window, true);


	float speed = cameraSpeed * deltaTime;
	if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS)
		cameraGO->getTransform()->addPosition(cameraGO->getTransform()->getForwardVector() * speed);
	if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS)
		cameraGO->getTransform()->addPosition(-cameraGO->getTransform()->getForwardVector() * speed);
	if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS)
		cameraGO->getTransform()->addPosition(-cameraGO->getTransform()->getRightVector() * speed);
	if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS)
		cameraGO->getTransform()->addPosition(cameraGO->getTransform()->getRightVector() * speed);

	if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraGO->getTransform()->addPosition(-World::getUpVector() * speed);
	if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS)
		cameraGO->getTransform()->addPosition(World::getUpVector() * speed);
}

void mouse_callback(GLFWwindow* Window, double x, double y)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	double xOffset = x - lastX;
	double yOffset = lastY - y;
	lastX = x;
	lastY = y;

	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	float yaw = cameraGO->getTransform()->getRotation().y;
	float pitch = cameraGO->getTransform()->getRotation().x;

	yaw += (float)xOffset;
	pitch += (float)yOffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	cameraGO->getTransform()->setRotation(Vector3(pitch, yaw, cameraGO->getTransform()->getRotation().z));
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

	glfwSetCursorPosCallback(window, mouse_callback);

	if (glewInit() != GLEW_OK)
	{
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	std::cout << glGetString(GL_VERSION) << std::endl;

	Shader* shader = new Shader("Resources/Shaders/Mesh-vs.shader", "Resources/Shaders/Mesh-fs.shader");


	cameraGO = World::getInstance().getActiveCamera()->getOwner();

	cameraGO->getTransform()->setPosition(Vector3(0.0f, 0.0f, 10.0f));
	cameraGO->getTransform()->setRotation(Vector3(0.0f, -90.0f, 0.0f));

	GameObject g0("g0");
	g0.addComponent(new Mesh("Resources/Models/Cube/Cube.obj"));

	float sc = 0.8f;
	g0.getTransform()->setPosition(Vector3(0.0f, 2.0f, 0.0f));
	g0.getTransform()->setScale(Vector3(sc, sc, sc));
	g0.getTransform()->setRotation(Vector3(45.0f, 0.0f, 0.0f));

	World::getInstance().addGameObject(&g0);

	World::getInstance().init();

	while (!glfwWindowShouldClose(window))
	{
		double currentFrame = glfwGetTime();
		deltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		World::getInstance().update(deltaTime);

		World::getInstance().render(deltaTime, shader);

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
