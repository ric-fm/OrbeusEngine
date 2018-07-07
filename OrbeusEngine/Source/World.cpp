#include "World.h"

#include "GameObject.h"
#include "Camera.h"

#include "Input.h"


World::World()
{
	GameObject* cameraGO = new GameObject("Camera");
	activeCamera = new Camera();
	cameraGO->addComponent<Camera>(activeCamera);

	addGameObject(cameraGO);
}

World::~World()
{
}

void World::addGameObject(GameObject* gameObject)
{
	gameObjects.push_back(gameObject);
}

void World::init()
{
	for (unsigned int i = 0; i < gameObjects.size(); ++i)
	{
		gameObjects[i]->init();
	}
}

void World::update(float deltaTime, Input* input)
{
	for (unsigned int i = 0; i < gameObjects.size(); ++i)
	{
		gameObjects[i]->update(deltaTime, input);
	}
}

void World::render(float deltaTime, Shader* shader)
{
	for (unsigned int i = 0; i < gameObjects.size(); ++i)
	{
		gameObjects[i]->render(deltaTime, shader);
	}
}