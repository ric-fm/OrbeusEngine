#include "GameObject.h"

#include "GameComponent.h"
#include "Transform.h"
#include "Rendering/Shader.h"


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

void GameObject::init()
{
	std::vector<Transform*> transforms = transform->getChildren();
	for (unsigned int i = 0; i < transforms.size(); ++i)
	{
		transforms[i]->getOwner()->init();
	}

	for (auto it = components.begin(); it != components.end(); ++it)
	{
		it->second->init();
	}
}

void GameObject::update(float deltaTime)
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		it->second->update(deltaTime);
	}

	std::vector<Transform*> transforms = transform->getChildren();
	for (unsigned int i = 0; i < transforms.size(); ++i)
	{
		transforms[i]->getOwner()->update(deltaTime);
	}
}

void GameObject::render(float deltaTime, Shader* shader)
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		it->second->render(deltaTime, shader);
	}

	std::vector<Transform*> transforms = transform->getChildren();
	for (unsigned int i = 0; i < transforms.size(); ++i)
	{
		transforms[i]->getOwner()->render(deltaTime, shader);
	}
}
