#pragma once

#include <vector>

#include "Orbeus/Math/Vector3.h"
#include "Orbeus/Math/Matrix4.h"
#include "Orbeus/Math/Quaternion.h"


class GameObject;

class Transform
{
private:
	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	Transform* parent = nullptr;
	std::vector<Transform*> children;

	GameObject* owner = nullptr;

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

	void setRelativePosition(const Vector3& position) { this->position = position; }
	Vector3 getRelativePosition() const { return position; }
	
	void addPosition(const Vector3& position) { this->position += position; }

	void setRelativeRotation(const Quaternion& rotation);
	Quaternion getRelativeRotation() const { return rotation; }

	void setRelativeScale(const Vector3& scale) { this->scale = scale; }
	Vector3 getRelativeScale() const { return scale; }

	Vector3 getPosition() const;
	Vector3 getScale() const;
	Quaternion getRotation() const;

	Vector3 transformVector(const Vector3 & source) const;

	Vector3 getForwardVector() const;
	Vector3 getUpVector() const;
	Vector3 getRightVector() const;
};