#pragma once

#include <vector>

#include "Math/Vector3.h"

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

public:
	World(World const&) = delete;
	void operator=(World const&) = delete;

private:
	GameObject* activeCameraGO = nullptr;
	Camera* activeCamera = nullptr;
	std::vector<GameObject*> gameObjects;

public:
	~World();

	void setActiveCamera(Camera* camera) { activeCamera = camera; }
	Camera* getActiveCamera() const { return activeCamera; }

	void addGameObject(GameObject* gameObject);

	void init();
	void update(float deltaTime);
	void render(float deltaTime, Shader* shader);
};
