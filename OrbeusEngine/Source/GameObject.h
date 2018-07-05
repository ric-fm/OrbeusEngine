#pragma once

#include <string>
#include <vector>

class Transform;
class GameComponent;
class Shader;

class GameObject
{
private:
	std::string name;
	Transform* transform = nullptr;
	std::vector<GameComponent*> components;

public:
	GameObject(const std::string& name);
	~GameObject();

	const std::string& getName() const { return name; }
	void setName(const std::string& name) { this->name = name; }

	Transform* getTransform() const { return transform; }

	void getChildren(std::vector<GameObject*>& children);
	void getAllChildren(std::vector<GameObject*>& children);

	void addComponent(GameComponent* component);
	std::vector<GameComponent*> getComponents() { return components; }

	virtual void init();
	virtual void update(float deltaTime);
	virtual void render(float deltaTime, Shader* shader);
};