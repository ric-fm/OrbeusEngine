#pragma once

#include <vector>

#include "Math/Vector3.h"
#include "Math/Matrix4.h"


class GameObject;

class Transform
{
private:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Vector3 forward;
	Vector3 up;
	Vector3 right;

	Transform* parent = nullptr;
	std::vector<Transform*> children;

	GameObject* owner = nullptr;

	void updateVectors();

public:
	Transform(GameObject* owner = nullptr);

protected:
	void addChild(Transform* child)
	{
		children.push_back(child);
	}

	void removeChild(Transform* child)
	{
		// TODO: Implement this
	}

public:
	void setParent(Transform* parent)
	{
		if (parent != nullptr)
		{
			parent->addChild(this);
		}
		else if(this->parent != nullptr)
		{
			this->parent->removeChild(this);
		}
		this->parent = parent;
	}

	std::vector<Transform*>& getChildren() { return children; }
	Transform* getParent() { return parent; }

	GameObject* getOwner() { return owner; }

	Matrix4 getMatrix() const;

	void setPosition(const Vector3& position) { this->position = position; }
	void addPosition(const Vector3& position) { this->position += position; }
	Vector3 getPosition() const { return position; }

	void setRotation(const Vector3& rotation);
	Vector3 getRotation() const { return rotation; }

	void setScale(const Vector3& scale) { this->scale = scale; }
	Vector3 getScale() const { return scale; }

	void lookAt(const Vector3& target);

	Vector3 getForwardVector() const { return forward; }
	Vector3 getUpVector() const { return up; }
	Vector3 getRightVector() const { return right; }
};