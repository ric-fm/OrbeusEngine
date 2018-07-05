#include "GameObject.h"

#include "GameComponent.h"
#include "Transform.h"
#include "Shader.h"


GameObject::GameObject(const std::string& name)
	: name(name)
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

void GameObject::addComponent(GameComponent* component)
{
	components.push_back(component);
	component->setOwner(this);
}

void GameObject::init()
{
	std::vector<Transform*> transforms = transform->getChildren();
	for (unsigned int i = 0; i < transforms.size(); ++i)
	{
		transforms[i]->getOwner()->init();
	}

	for (unsigned int i = 0; i < components.size(); ++i)
	{
		components[i]->init();
	}
}

void GameObject::update(float deltaTime)
{
	for (unsigned int i = 0; i < components.size(); ++i)
	{
		components[i]->update(deltaTime);
	}

	std::vector<Transform*> transforms = transform->getChildren();
	for (unsigned int i = 0; i < transforms.size(); ++i)
	{
		transforms[i]->getOwner()->update(deltaTime);
	}
}

void GameObject::render(float deltaTime, Shader* shader)
{
	for (unsigned int i = 0; i < components.size(); ++i)
	{
		components[i]->render(deltaTime, shader);
	}

	std::vector<Transform*> transforms = transform->getChildren();
	for (unsigned int i = 0; i < transforms.size(); ++i)
	{
		transforms[i]->getOwner()->render(deltaTime, shader);
	}
}
