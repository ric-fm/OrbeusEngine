#pragma once

#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "Math/Vector3.h"
#include "GameComponent.h"

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

private:
	GameObject* activeCameraGO = nullptr;
	Camera* activeCamera = nullptr;
	std::vector<GameObject*> gameObjects;

	std::unordered_map<std::type_index, std::vector<GameComponent*>> components;

public:
	~World();

	void setActiveCamera(Camera* camera) { activeCamera = camera; }
	Camera* getActiveCamera() const { return activeCamera; }

	template<class ComponentType>
	void registerComponent(GameComponent* component)
	{
		component->ID = ++componentCount;
		components[std::type_index(typeid(ComponentType))].push_back(component);
	}

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
	void render(float deltaTime, Shader* shader);
};
