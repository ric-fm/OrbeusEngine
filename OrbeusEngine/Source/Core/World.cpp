#include "World.h"

#include "GameObject.h"
#include "Input.h"

#include "Components/Camera.h"


unsigned int World::objectCount = 0;
unsigned int World::componentCount = 0;

World::World()
{
	GameObject* cameraGO = new GameObject("Camera");
	activeCamera = new Camera();
	registerComponent<Camera>(activeCamera);
	cameraGO->addComponent<Camera>(activeCamera);
	addGameObject(cameraGO);
}

World::~World()
{
}

void World::addGameObject(GameObject* gameObject)
{
	gameObject->ID = ++objectCount;
	gameObjects.push_back(gameObject);
}

void World::init()
{
	for (unsigned int i = 0; i < gameObjects.size(); ++i)
	{
		gameObjects[i]->init();
	}
}

void World::update(float deltaTime)
{
	for (unsigned int i = 0; i < gameObjects.size(); ++i)
	{
		gameObjects[i]->update(deltaTime);
	}
}

void World::render(float deltaTime, Shader* shader)
{
	for (unsigned int i = 0; i < gameObjects.size(); ++i)
	{
		gameObjects[i]->render(deltaTime, shader);
	}
}
