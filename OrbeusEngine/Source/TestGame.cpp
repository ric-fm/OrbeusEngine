#include "TestGame.h"


#include "CameraController.h"
#include "Rotate.h"
#include "Shader.h"
#include "Window.h"
#include "Log.h"

#include "GLFW/glfw3.h"

void TestGame::init()
{
	//Shader* shader = new Shader("Resources/Shaders/Mesh-vs.shader", "Resources/Shaders/Mesh-fs.shader");
	Shader* shader = new Shader("Resources/Shaders/Phong-vs.shader", "Resources/Shaders/Phong-fs.shader");
	Engine::getInstance().getRenderingEngine()->setShader(shader);

	Engine::getInstance().getWindow()->setCursorMode(GLFW_CURSOR_DISABLED);

	GameObject* cameraGO = World::getInstance().getActiveCamera()->getOwner();

	cameraGO->getTransform()->setPosition(Vector3(0.0f, 0.0f, 10.0f));
	cameraGO->getTransform()->setRotation(Vector3(0.0f, -90.0f, 0.0f));

	cameraGO->addComponent<CameraController>(new CameraController());

	GameObject* cube = new GameObject("Cube");
	cube->addComponent<Mesh>(new Mesh("Resources/Models/Cube/Cube.obj"));
	cube->addComponent<Rotate>(new Rotate(Vector3(0.0f, 20.0f, 0.0f)));

	//cube->getTransform()->setPosition(Vector3(0.0f, 2.0f, 0.0f));
	//cube->getTransform()->setScale(Vector3(0.8f, 0.8f, 0.8f));
	cube->getTransform()->setRotation(Vector3(45.0f, 0.0f, 0.0f));

	World::getInstance().addGameObject(cube);
}

void TestGame::update(float deltaTime)
{
	if (Input::isKeyDown(GLFW_KEY_ESCAPE))
	{
		Engine::getInstance().quitGame();
	}
}
