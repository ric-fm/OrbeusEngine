#include "TestGame.h"

#include "Window/Window.h"
#include "Components/Light.h"
#include "Components/Mesh.h"
#include "Components/GUIText.h"
#include "Utils/Log.h"
#include "CameraController.h"
#include "Rotate.h"
#include "CopyTransform.h"

#include "GLFW/glfw3.h"

#include "Math/Math.h"

#include "Core/GameComponent.h"
#include "Test/MoveController.h"

#include "Rendering/Text/TextRenderer.h"

GameObject* textGO = nullptr;


void TestGame::init()
{
	Engine::getInstance().getWindow()->setCursorMode(GLFW_CURSOR_DISABLED);

	Engine::getInstance().getRenderingEngine()->setAmbienLight(Vector3(0.2f, 0.2f, 0.2f));

	GameObject* cameraGO = World::getInstance().getActiveCamera()->getOwner();

	cameraGO->getTransform()->setRelativePosition(Vector3(0.0f, 3.0f, -8.0f));

	cameraGO->addComponent<CameraController>(new CameraController());


	GameObject* cube = new GameObject("Cube");
	cube->getTransform()->setRelativePosition(Vector3(2.0f, 1.0f, 0.0f));

	Mesh* cubeMesh = new Mesh("Resources/Models/Cube/Cube.obj");
	World::getInstance().registerComponent<Mesh>(cubeMesh);
	cube->addComponent<Mesh>(cubeMesh);


	GameObject* monkey1 = new GameObject("Monkey");
	GameObject* monkeyMesh1 = new GameObject("Monkey");
	monkeyMesh1->getTransform()->setParent(monkey1->getTransform());

	Mesh* monkeyMesh = new Mesh("Resources/Models/Monkey/Monkey.obj");
	World::getInstance().registerComponent<Mesh>(monkeyMesh);
	monkeyMesh1->addComponent<Mesh>(monkeyMesh);
	monkey1->getTransform()->setRelativePosition(Vector3(0.0f, 3.0f, 2.0f));
	monkey1->getTransform()->setRelativeScale(Vector3(2.0f, 2.0f, 2.0f));

	monkey1->addComponent<MoveController>(new MoveController());

	GameObject* monkey2 = new GameObject("Monkey");
	Mesh* monkey2Mesh = new Mesh("Resources/Models/Monkey/Monkey.obj");
	World::getInstance().registerComponent<Mesh>(monkey2Mesh);
	monkey2->addComponent<Mesh>(monkey2Mesh);
	monkey2->getTransform()->setRelativePosition(Vector3(0.0f, 0.0f, -2.0f));
	monkey2->getTransform()->setParent(monkey1->getTransform());


	GameObject* monkey3 = new GameObject("Monkey");
	Mesh* monkey3Mesh = new Mesh("Resources/Models/Monkey/Monkey.obj");
	World::getInstance().registerComponent<Mesh>(monkey3Mesh);
	monkey3->addComponent<Mesh>(monkey3Mesh);
	monkey3->getTransform()->setRelativePosition(Vector3(0.0f, 1.0f, -2.0f));



	GameObject* floor = new GameObject("Floor");
	floor->getTransform()->setRelativeScale(Vector3(10.0f));
	Mesh* floorMesh = new Mesh("Resources/Models/Plane/Plane.obj");
	World::getInstance().registerComponent<Mesh>(floorMesh);
	floor->addComponent<Mesh>(floorMesh);


	World::getInstance().addGameObject(floor);
	World::getInstance().addGameObject(cube);
	World::getInstance().addGameObject(monkey1);
	//World::getInstance().addGameObject(monkey2);
	//World::getInstance().addGameObject(monkey3);


	GameObject* directionalGO0 = new GameObject("Directional0");
	directionalGO0->addComponent<DirectionalLight>(new DirectionalLight(Vector3(1.0f, 0.0f, 0.0f), 0.8f));
	directionalGO0->getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(60, 20, 0)));

	GameObject* directionalGO1 = new GameObject("Directional1");
	directionalGO1->addComponent<DirectionalLight>(new DirectionalLight(Vector3(0.0f, 1.0f, 0.0f), 0.9f));

	GameObject* pointGO0 = new GameObject("PointLight0");
	pointGO0->addComponent<PointLight>(new PointLight(Vector3(1.0f, 0.0f, 0.0f), 0.5f, 1.0f, 0.09f, 0.032f, 5.0f));
	pointGO0->getTransform()->setRelativePosition(Vector3(2.0f, 1.0f, 5.0f));

	GameObject* pointGO1 = new GameObject("PointLight1");
	pointGO1->addComponent<PointLight>(new PointLight(Vector3(0.0f, 0.0f, 1.0f), 0.5f, 1.0f, 0.09f, 0.032f, 5.0f));
	pointGO1->getTransform()->setRelativePosition(Vector3(5.0f, 1.0f, 0.0f));

	GameObject* spotGO = new GameObject("SpotLight");
	spotGO->addComponent<SpotLight>(new SpotLight(Vector3(0.0f, 1.0f, 0.0f), 0.8f, 0.0f, 0.0f, 0.2f, 20.0f, 0.8f));
	spotGO->getTransform()->setParent(cameraGO->getTransform());

	World::getInstance().addGameObject(directionalGO0);
	World::getInstance().addGameObject(directionalGO1);
	World::getInstance().addGameObject(pointGO0);
	World::getInstance().addGameObject(pointGO1);
	World::getInstance().addGameObject(spotGO);


	textGO = new GameObject("Text");
	guiText = new GUIText("Hello World!");
	textGO->addComponent<GUIText>(guiText);
	World::getInstance().registerComponent<GUIText>(guiText);

	World::getInstance().addGameObject(textGO);

	GameObject* textGO2 = new GameObject("Text2");
	textGO2->getTransform()->setRelativePosition(Vector3(0.5f, 0.5f, 0.0f));
	GUIText* guiText2 = new GUIText("Text", 3);
	guiText2->setColor(Vector3(1, 0, 0));
	textGO2->addComponent<GUIText>(guiText2);
	World::getInstance().registerComponent<GUIText>(guiText2);

	World::getInstance().addGameObject(textGO2);


}

void TestGame::update(float deltaTime)
{
	if (Input::isKeyDown(GLFW_KEY_ESCAPE))
	{
		Engine::getInstance().quitGame();
	}


	if (Input::isKeyDown(GLFW_KEY_G))
	{
		guiText->setText("Text2");
	}

}
