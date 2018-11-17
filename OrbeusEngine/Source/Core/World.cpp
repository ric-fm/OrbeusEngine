#include "World.h"

#include "GameObject.h"
#include "Transform.h"
#include "Input.h"

#include "Components/Camera.h"


unsigned int World::objectCount = 0;
unsigned int World::componentCount = 0;

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

void World::initGameObjectHierarchy(GameObject* gameObject)
{
	// Init gameObject
	gameObject->init();

	// Init gameObject components
	for (auto it = gameObject->components.begin(); it != gameObject->components.end(); ++it)
	{
		it->second->init();
	}

	// Init children
	std::vector<Transform*> transforms = gameObject->transform->getChildren();
	for (unsigned int i = 0; i < transforms.size(); ++i)
	{
		//transforms[i]->getOwner()->init();
		initGameObjectHierarchy(transforms[i]->getOwner());
	}
}

void World::updateGameObjectHierarchy(GameObject* gameObject, float deltaTime)
{
	// Init gameObject
	gameObject->update(deltaTime);

	// Update gameObject components
	for (auto it = gameObject->components.begin(); it != gameObject->components.end(); ++it)
	{
		it->second->update(deltaTime);
	}

	// Update children
	std::vector<Transform*> transforms = gameObject->transform->getChildren();
	for (unsigned int i = 0; i < transforms.size(); ++i)
	{
		//transforms[i]->getOwner()->update(deltaTime);
		updateGameObjectHierarchy(transforms[i]->getOwner(), deltaTime);
	}

}

void World::addGameObject(GameObject* gameObject)
{
	gameObject->ID = ++objectCount;
	gameObjects.push_back(gameObject);

	for (auto componentsIt = gameObject->components.begin(); componentsIt != gameObject->components.end(); ++componentsIt)
	{
		components[componentsIt->first].push_back(componentsIt->second);
	}
}

void World::init()
{
	for (unsigned int i = 0; i < gameObjects.size(); ++i)
	{
		if (gameObjects[i]->getTransform()->getParent() == nullptr)
		{
			initGameObjectHierarchy(gameObjects[i]);
		}
	}
}

void World::update(float deltaTime)
{
	for (unsigned int i = 0; i < gameObjects.size(); ++i)
	{
		// Update gameObjects with no parent. The children are updated by his parent
		if (gameObjects[i]->getTransform()->getParent() == nullptr)
		{
			updateGameObjectHierarchy(gameObjects[i], deltaTime);
		}
	}
}
