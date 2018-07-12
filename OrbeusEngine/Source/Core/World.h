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


	static Vector3 getForwardVector() { return Vector3(0.0f, 0.0f, 1.0f); }
	static Vector3 getUpVector() { return Vector3(0.0f, 1.0f, 0.0f); }
	static Vector3 getRightVector() { return Vector3(1.0f, 0.0f, 0.0f); }
};
