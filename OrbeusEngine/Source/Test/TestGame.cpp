#include "TestGame.h"

#include "Window/Window.h"
#include "Components/Light.h"
#include "Components/Mesh.h"
#include "Utils/Log.h"
#include "CameraController.h"
#include "Rotate.h"
#include "CopyTransform.h"

#include "GLFW/glfw3.h"

#include "Math/Math.h"

void TestGame::init()
{
	Engine::getInstance().getWindow()->setCursorMode(GLFW_CURSOR_DISABLED);

	Engine::getInstance().getRenderingEngine()->setAmbienLight(Vector3(0.2f, 0.2f, 0.2f));

	GameObject* cameraGO = World::getInstance().getActiveCamera()->getOwner();

	cameraGO->getTransform()->setPosition(Vector3(0.0f, 3.0f, 8.0f));
	cameraGO->getTransform()->setRotation(Vector3(0.0f, -90.0f, 0.0f));

	cameraGO->addComponent<CameraController>(new CameraController());

	GameObject* cube = new GameObject("Cube");
	cube->getTransform()->setPosition(Vector3(0.0f, 1.0f, 0.0f));
	cube->addComponent<Mesh>(new Mesh("Resources/Models/Cube/Cube.obj"));
	cube->addComponent<Rotate>(new Rotate(Vector3(0.0f, 20.0f, 0.0f)));
	World::getInstance().addGameObject(cube);

	GameObject* floor = new GameObject("Floor");
	floor->getTransform()->setRotation(Vector3(0.0f, 0.0f, 0.0f));
	floor->getTransform()->setScale(Vector3(10.0f));
	floor->addComponent<Mesh>(new Mesh("Resources/Models/Plane/Plane.obj"));
	World::getInstance().addGameObject(floor);

	floor->getTransform()->setParent(cube->getTransform());
	floor->getTransform()->setPosition(Vector3(0.0f, -1.0f, 0.0f));

	GameObject* directionalGO0 = new GameObject("Directional0");
	directionalGO0->addComponent<DirectionalLight>(new DirectionalLight(Vector3(1.0f, 0.0f, 0.0f), 0.8f));
	directionalGO0->getTransform()->setRotation(Vector3(-70.0f, 0.0f, 0.0f));

	GameObject* directionalGO1 = new GameObject("Directional1");
	directionalGO1->addComponent<DirectionalLight>(new DirectionalLight(Vector3(0.0f, 1.0f, 0.0f), 0.9f));
	directionalGO1->getTransform()->setRotation(Vector3(-110.0f, 0.0f, 0.0f));

	GameObject* pointGO0 = new GameObject("PointLight0");
	pointGO0->addComponent<PointLight>(new PointLight(Vector3(1.0f, 0.0f, 0.0f), 0.5f, 1.0f, 0.09f, 0.032f, 5.0f));
	pointGO0->getTransform()->setPosition(Vector3(0.0f, 1.0f, 5.0f));

	GameObject* pointGO1 = new GameObject("PointLight1");
	pointGO1->addComponent<PointLight>(new PointLight(Vector3(0.0f, 0.0f, 1.0f), 0.5f, 1.0f, 0.09f, 0.032f, 5.0f));
	pointGO1->getTransform()->setPosition(Vector3(5.0f, 1.0f, 0.0f));

	GameObject* spotGO = new GameObject("SpotLight");
	spotGO->addComponent<SpotLight>(new SpotLight(Vector3(0.0f, 1.0f, 0.0f), 0.8f, 0.0f, 0.0f, 0.2f, 20.0f, 0.8f));
	spotGO->getTransform()->setParent(cameraGO->getTransform());
	//spotGO->getTransform()->setPosition(Vector3(0.0f, 0.0f, 0.0f));
	spotGO->getTransform()->setRotation(Vector3(0.0f, -90.0f, 0.0f));

	World::getInstance().addGameObject(directionalGO0);
	World::getInstance().addGameObject(directionalGO1);
	World::getInstance().addGameObject(pointGO0);
	World::getInstance().addGameObject(pointGO1);
	World::getInstance().addGameObject(spotGO);

	Matrix4 t;

	Vector3 pos(1.0f, 2.0f, 3.0f);

	Matrix4 tWorld = t.translate(pos);

	Log::info("%s", tWorld.toString().c_str());
}

void TestGame::update(float deltaTime)
{
	if (Input::isKeyDown(GLFW_KEY_ESCAPE))
	{
		Engine::getInstance().quitGame();
	}
}
