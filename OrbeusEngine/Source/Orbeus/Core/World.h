#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "Orbeus/Math/Vector3.h"
#include "Orbeus/Core/GameComponent.h"

class Camera;
class GameObject;
class Shader;
class Input;


class World
{
public:
	static World& getInstance()
	{
		static World instance;

		return instance;
	}
private:
	World();

	static unsigned int objectCount;
	static unsigned int componentCount;

public:
	World(World const&) = delete;
	void operator=(World const&) = delete;

public:
	~World();

private:
	GameObject* activeCameraGO = nullptr;
	Camera* activeCamera = nullptr;
	std::vector<GameObject*> gameObjects;

	std::unordered_map<std::type_index, std::vector<GameComponent*>> components;

	void initGameObjectHierarchy(GameObject* gameObject);
	void updateGameObjectHierarchy(GameObject* gameObject, float deltaTime);

public:
	void setActiveCamera(Camera* camera) { activeCamera = camera; }
	Camera* getActiveCamera() const { return activeCamera; }

	template<class ComponentType>
	std::vector<ComponentType*> getComponents()
	{
		std::vector<ComponentType*> result;
		std::type_index componentType(typeid(ComponentType));


		for (unsigned int i = 0; i < components[componentType].size(); ++i)
		{
			result.push_back(static_cast<ComponentType*>(components[componentType][i]));
		}

		return result;
	}

	void addGameObject(GameObject* gameObject);

	void init();
	void update(float deltaTime);
};
