#include "GameObject.h"

#include "Orbeus/Core/GameComponent.h"
#include "Orbeus/Core/Transform.h"
#include "Orbeus/Rendering/Shader.h"


GameObject::GameObject(const std::string& name)
	: ID(-1), name(name)
{
	transform = new Transform(this);
}

GameObject::~GameObject()
{
}

void GameObject::getChildren(std::vector<GameObject*>& children)
{
	children.clear();

	std::vector<Transform*> transforms = transform->getChildren();

	for (unsigned int i = 0; i < transforms.size(); ++i)
	{
		children.push_back(transforms[i]->getOwner());
	}
}

void GameObject::getAllChildren(std::vector<GameObject*>& children)
{
	std::vector<Transform*> transforms = transform->getChildren();

	for (unsigned int i = 0; i < transforms.size(); ++i)
	{
		children.push_back(transforms[i]->getOwner());

		std::vector<GameObject*> childrenOfChild;
		transforms[i]->getOwner()->getAllChildren(childrenOfChild);
		children.insert(children.end(), childrenOfChild.begin(), childrenOfChild.end());
	}
}

std::vector<GameComponent*> GameObject::getComponents()
{
	std::vector<GameComponent*> result;
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		result.push_back(it->second);
	}
	return result;
}
