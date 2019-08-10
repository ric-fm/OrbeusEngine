#include "TestGame.h"

#include "Orbeus/Window/Window.h"
#include "Orbeus/Components/Light.h"
#include "Orbeus/Components/Mesh.h"
#include "Orbeus/Components/GUIText.h"
#include "Orbeus/Components/Terrain.h"
#include "Orbeus/Components/WaterPlane.h"
#include "Orbeus/Components/CameraCapture.h"
#include "Orbeus/Components/Mesh.h"
#include "Orbeus/Components/SkeletalMesh.h"
#include "Orbeus/Utils/Log.h"
#include "CameraController.h"
#include "Rotate.h"
#include "CopyTransform.h"

#include "GLFW/glfw3.h"

#include "Orbeus/Math/Math.h"

#include "Orbeus/Core/GameComponent.h"
#include "Test/MoveController.h"

#include "Orbeus/ResourceManagement/ResourceManager.h"

GameObject* textGO = nullptr;


void TestGame::preInit()
{
	World::getInstance().getActiveCamera()->setAmbienLight(Vector3(0.2f, 0.2f, 0.2f));
	World::getInstance().getActiveCamera()->setSkyColor(Vector3(0.1f, 0.9f, 1.0f));
	World::getInstance().getActiveCamera()->setFogColor(Vector3(0.94f, 0.93f, 0.92f));
	World::getInstance().getActiveCamera()->setFogDensity(0.008f);

	GameObject* cameraGO = World::getInstance().getActiveCamera()->getOwner();

	cameraGO->getTransform()->setRelativePosition(Vector3(0.0f, 3.0f, -8.0f));

	CameraController* cameraController = cameraGO->addComponent<CameraController>(new CameraController());


	//GameObject* cube = new GameObject("Cube");
	//cube->getTransform()->setRelativePosition(Vector3(2.0f, 1.0f, 0.0f));

	////Mesh* cubeMesh = new Mesh("Resources/Models/Cube/Cube.obj");
	////cube->addComponent<Mesh>(cubeMesh);


	//GameObject* monkey1 = new GameObject("Monkey");
	//GameObject* monkeyMesh1 = new GameObject("Monkey");
	//monkeyMesh1->getTransform()->setParent(monkey1->getTransform());

	//Mesh* monkeyMesh = new Mesh("Resources/Models/Monkey/Monkey.obj");
	//monkeyMesh1->addComponent<Mesh>(new Mesh("Resources/Models/Monkey/Monkey.obj"));
	//monkey1->getTransform()->setRelativePosition(Vector3(0.0f, 3.0f, 2.0f));
	//monkey1->getTransform()->setRelativeScale(Vector3(2.0f, 2.0f, 2.0f));

	//monkey1->addComponent<MoveController>(new MoveController());


	////GameObject* floor = new GameObject("Floor");
	////floor->getTransform()->setRelativeScale(Vector3(10.0f));
	////Mesh* floorMesh = new Mesh("Resources/Models/Plane/Plane.obj");
	////floor->addComponent<Mesh>(floorMesh);


	////World::getInstance().addGameObject(floor);
	////World::getInstance().addGameObject(cube);
	////World::getInstance().addGameObject(monkey1);
	////World::getInstance().addGameObject(monkeyMesh1);


	GameObject* directionalGO0 = new GameObject("Directional0");
	directionalGO0->addComponent<DirectionalLight>(new DirectionalLight(Vector3(1.0f, 1.0f, 1.0f), 0.8f));
	directionalGO0->getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(60, 180, 0)));

	GameObject* directionalGO1 = new GameObject("Directional1");
	directionalGO1->addComponent<DirectionalLight>(new DirectionalLight(Vector3(0.0f, 1.0f, 0.0f), 0.9f));

	GameObject* pointGO0 = new GameObject("PointLight0");
	pointGO0->addComponent<PointLight>(new PointLight(Vector3(1.0f, 0.0f, 0.0f), 0.5f, 1.0f, 0.09f, 0.032f, 5.0f));
	pointGO0->getTransform()->setParent(cameraGO->getTransform());
	pointGO0->getTransform()->setRelativePosition(Vector3(-1.0f, -1.0f, 0.0f));

	GameObject* pointGO1 = new GameObject("PointLight1");
	pointGO1->addComponent<PointLight>(new PointLight(Vector3(0.0f, 0.0f, 1.0f), 0.5f, 1.0f, 0.09f, 0.032f, 25.0f));
	pointGO1->getTransform()->setParent(cameraGO->getTransform());
	pointGO1->getTransform()->setRelativePosition(Vector3(1.0f, -1.0f, 0.0f));

	GameObject* spotGO = new GameObject("SpotLight");
	spotGO->addComponent<SpotLight>(new SpotLight(Vector3(0.0f, 1.0f, 0.0f), 0.8f, 0.0f, 0.0f, 0.2f, 20.0f, 0.8f));
	spotGO->getTransform()->setParent(cameraGO->getTransform());

	World::getInstance().addGameObject(directionalGO0);
	World::getInstance().addGameObject(directionalGO1);
	World::getInstance().addGameObject(pointGO0);
	World::getInstance().addGameObject(pointGO1);
	World::getInstance().addGameObject(spotGO);

	//GameObject* terrainGO = new GameObject("terrain");
	//Terrain* terrain = new Terrain("Resources/Textures/Terrain/grassy2.png", "Resources/Textures/Terrain/mud.png",
	//	"Resources/Textures/Terrain/grassFlowers.png", "Resources/Textures/Terrain/path.png", "Resources/Textures/Terrain/blendMap.png",
	//	"Resources/Textures/Terrain/heightmap.png");
	//terrainGO->addComponent<Terrain>(terrain);
	////World::getInstance().addGameObject(terrainGO);


	//GameObject* grassGO = new GameObject("Grass");
	//grassGO->getTransform()->setRelativePosition(Vector3(0, 0, 0));
	//grassGO->addComponent<Mesh>(new Mesh("Resources/Models/Grass/Grass.obj", true));
	////World::getInstance().addGameObject(grassGO);

	//GameObject* grassGO2 = new GameObject("Grass2");
	//grassGO2->getTransform()->setRelativePosition(Vector3(0.5f, 0, 1));
	//grassGO2->addComponent<Mesh>(new Mesh("Resources/Models/Grass/Grass.obj", true));
	////World::getInstance().addGameObject(grassGO2);

	//GameObject* textgo = new GameObject("Text");
	//textgo->addComponent<GUIText>(new GUIText("Orbeus Engine", 2));
	//textgo->getTransform()->setRelativePosition(Vector3(0.4f, 0.0f, 0.0f));
	////World::getInstance().addGameObject(textgo);

	//WaterPlane* waterPlane = new WaterPlane();
	//waterGO = new GameObject("Water");
	//waterGO->addComponent<WaterPlane>(waterPlane);
	////waterGO->getTransform()->setRelativePosition(Vector3(0.0f, 0.0f, 4.0f));
	//waterGO->getTransform()->setRelativePosition(Vector3(78.0f, -12.0f, 26.0f));
	//waterGO->getTransform()->setRelativeScale(Vector3(200.0f, 1.0f, 200.0f));


	//CameraCapture* reflectionCapture = new CameraCapture(320, 180);
	//waterReflectionGO = new GameObject("WaterReflection");
	//waterReflectionGO->addComponent<CameraCapture>(reflectionCapture);

	//CameraCapture* refractionCapture = new CameraCapture(1280, 720);
	//waterRefractionGO = new GameObject("WaterRefraction");
	//waterRefractionGO->addComponent<CameraCapture>(refractionCapture);
	//
	//waterPlane->setNormalMap(ResourceManager::getInstance().getTexture("Resources/Textures/Water/NormalMap.png", "normal"));
	//waterPlane->setDudvMap(ResourceManager::getInstance().getTexture("Resources/Textures/Water/DUDV.png", "map"));

	////World::getInstance().addGameObject(waterRefractionGO);
	////World::getInstance().addGameObject(waterReflectionGO);
	////World::getInstance().addGameObject(waterGO);



	//cameraGO->getTransform()->setRelativePosition(Vector3(0.0f, 1.0f, -3.0f));

	//GameObject* cube = new GameObject("Cube");
	//cube->getTransform()->setRelativePosition(Vector3(-2, 0, 0));
	//cube->addComponent<Cube>(new Cube());
	//World::getInstance().addGameObject(cube);

	//GameObject* cubeOBJ = new GameObject("Cube OBJ");
	//cubeOBJ->getTransform()->setRelativePosition(Vector3(2, 0, 0));
	////cubeOBJ->getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(0, 180, 0)));
	//cubeOBJ->addComponent<Mesh>(new Mesh("Resources/Models/Animation/Triangle.obj"));
	//World::getInstance().addGameObject(cubeOBJ);


	//GameObject* cubeFBX = new GameObject("Cube FBX");
	//cubeFBX->getTransform()->setRelativePosition(Vector3(-2, 0, 0));
	////cubeFBX->getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(0, 180, 0)));
	////cubeFBX->getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(-90, 0, 0)));
	//cubeFBX->addComponent<SkeletalMesh>(new SkeletalMesh("Resources/Models/Animation/Triangle.fbx"));
	////cubeFBX->addComponent<SkeletalMesh>(new SkeletalMesh("Resources/Models/Animation/Triangle2.fbx"));
	////cubeFBX->addComponent<SkeletalMesh>(new SkeletalMesh("Resources/Models/Animation/max.fbx"));
	//World::getInstance().addGameObject(cubeFBX);


	//GameObject* cubeFBX2 = new GameObject("Cube FBX2");
	//cubeFBX2->getTransform()->setRelativePosition(Vector3(2, 0, 0));
	////cubeFBX->getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(0, 180, 0)));
	////cubeFBX->getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(-90, 0, 0)));
	////cubeFBX->addComponent<SkeletalMesh>(new SkeletalMesh("Resources/Models/Animation/Triangle.fbx"));
	//cubeFBX2->addComponent<SkeletalMesh>(new SkeletalMesh("Resources/Models/Animation/Triangle2.fbx"));
	////cubeFBX->addComponent<SkeletalMesh>(new SkeletalMesh("Resources/Models/Animation/max.fbx"));
	//World::getInstance().addGameObject(cubeFBX2);



	GameObject* animatedGO = new GameObject("Animation Test");
	animatedGO->getTransform()->setRelativePosition(Vector3(-2, 0, 0));
	//animatedGO->getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(-90, 0, 0)));
	std::string fbxPath;
	fbxPath = "Resources/Models/Animation/test.fbx";
	//fbxPath = "Resources/Models/Animation/test2.fbx";
	//fbxPath = "Resources/Models/Plane/Plane.fbx";
	//fbxPath = "Resources/Models/Cube/Cube.fbx";
	//fbxPath = "Resources/Models/Animation/CubeAxis.fbx";

	animatedGO->addComponent<SkeletalMesh>(new SkeletalMesh(fbxPath));

	World::getInstance().addGameObject(animatedGO);

	GameObject* cubeOBJ = new GameObject("Cube OBJ");
	cubeOBJ->getTransform()->setRelativePosition(Vector3(2, 0, 0));
	//cubeOBJ->getTransform()->setRelativeRotation(Quaternion::EulerAngles(Vector3(0, 180, 0)));
	cubeOBJ->addComponent<Mesh>(new Mesh("Resources/Models/Animation/test.obj"));
	World::getInstance().addGameObject(cubeOBJ);
}

void TestGame::postInit()
{
	//WaterPlane* waterPlane = waterGO->getComponent<WaterPlane>();
	//CameraCapture* reflectionCapture = waterReflectionGO->getComponent<CameraCapture>();
	//CameraCapture* refractionCapture = waterRefractionGO->getComponent<CameraCapture>();

	//waterPlane->setReflectionTexture(reflectionCapture->getColorTexture());
	//waterPlane->setRefractionTexture(refractionCapture->getColorTexture());
	//waterPlane->setRefractionDepthTexture(refractionCapture->getDepthTexture());

	//reflectionCapture->setClipPlane(Vector4(0, 1, 0, -waterPlane->getTransform()->getRelativePosition().y + 1.0f));
	//refractionCapture->setClipPlane(Vector4(0, -1, 0, waterPlane->getTransform()->getRelativePosition().y + 1.0f));
}

void TestGame::update(float deltaTime)
{
	if (Input::isKeyDown(GLFW_KEY_ESCAPE))
	{
		Engine::getInstance().quitGame();
	}

	//if (Input::isKeyDown(GLFW_KEY_F2))
	//{
	//	gameMode = !gameMode;
	//	GameObject* cameraGO = World::getInstance().getActiveCamera()->getOwner();
	//	CameraController* cameraController = cameraGO->getComponent<CameraController>();
	//	if (gameMode)
	//		cameraController->setEnabled(true);
	//	else
	//		cameraController->setEnabled(false);
	//}
	if (Input::isKeyDown(GLFW_KEY_F3))
	{
		lockCursor = !lockCursor;
		if(lockCursor)
			Engine::getInstance().getWindow()->setCursorMode(GLFW_CURSOR_DISABLED);
		else
			Engine::getInstance().getWindow()->setCursorMode(GLFW_CURSOR_HIDDEN);
	}


	//WaterPlane* waterPlane = waterGO->getComponent<WaterPlane>();
	//CameraCapture* reflectionCapture = waterReflectionGO->getComponent<CameraCapture>();
	//CameraCapture* refractionCapture = waterRefractionGO->getComponent<CameraCapture>();

	//Camera* camera = World::getInstance().getActiveCamera();
	//Quaternion cameraRotation = camera->getTransform()->getRelativeRotation();
	//Vector3 cameraPosition = camera->getTransform()->getRelativePosition();
	//Vector3 invPitchRotation = cameraRotation.getEulerAngles();
	//invPitchRotation.x *= -1;

	//float offsetHeight = 2.0f * (camera->getTransform()->getPosition().y - waterPlane->getTransform()->getRelativePosition().y);

	//reflectionCapture->getTransform()->setRelativePosition(cameraPosition - Vector3(0, offsetHeight, 0));
	//reflectionCapture->getTransform()->setRelativeRotation(Quaternion::EulerAngles(invPitchRotation));
	//
	//refractionCapture->getTransform()->setRelativePosition(cameraPosition);
	//refractionCapture->getTransform()->setRelativeRotation(cameraRotation);

	//reflectionCapture->capture();
	//refractionCapture->capture();
}
