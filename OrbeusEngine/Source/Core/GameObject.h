#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

#include "GameComponent.h"



class Transform;
class Shader;

class GameObject
{
	friend class World;
private:
	int ID;

	std::string name;
	Transform* transform = nullptr;
	std::unordered_map<std::type_index, GameComponent*> components;


public:
	GameObject(const std::string& name);
	~GameObject();

	int getID() const { return ID; }

	const std::string& getName() const { return name; }
	void setName(const std::string& name) { this->name = name; }

	Transform* getTransform() const { return transform; }

	void getChildren(std::vector<GameObject*>& children);
	void getAllChildren(std::vector<GameObject*>& children);

	template<class ComponentType>
	inline void addComponent(GameComponent* component)
	{
		components[std::type_index(typeid(ComponentType))] = component;
		component->setOwner(this);
	}

	template<class ComponentType>
	ComponentType* getComponent()
	{
		std::type_index type(typeid(ComponentType));
		if (components.count(type) != 0)
		{
			return static_cast<ComponentType*>(components[type]);
		}
		return nullptr;
	}

	std::vector<GameComponent*> getComponents();

	virtual void init() {}
	virtual void update(float deltaTime) {}
};
