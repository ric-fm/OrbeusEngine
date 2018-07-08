#pragma once

#include <string>

#include "Input.h"


class Transform;
class GameObject;
class Shader;

class GameComponent
{
private:
	std::string name;
	GameObject* owner = nullptr;

public:
	GameComponent();
	~GameComponent();

	void setName(const std::string& name) { this->name = name; }
	const std::string& getName() const { return name; }

	void setOwner(GameObject* owner) { this->owner = owner; }
	GameObject* getOwner() const { return owner; }
	Transform* getTransform() const;

	virtual void init() { }
	virtual void update(float deltaTime) { }
	virtual void render(float deltaTime, Shader* shader) { }
};
