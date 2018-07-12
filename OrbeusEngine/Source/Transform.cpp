#include "Transform.h"
#include "Math.h"

#include "World.h"
#include "GameObject.h"


void Transform::updateVectors()
{
	float pitch = Math::degreesToRadians(rotation.x);
	float yaw = Math::degreesToRadians(rotation.y);
	float roll = Math::degreesToRadians(rotation.z);

	float x = cos(pitch) * cos(yaw);
	float y = sin(pitch);
	float z = cos(pitch) * sin(yaw);

	forward = Vector3(x, y, z).normalize();
	right = forward.cross(World::getUpVector()).normalize();
	up = right.cross(forward);
}

Transform::Transform(GameObject* owner)
	: owner(owner), position(0.0f, 0.0f, 0.0f), rotation(0.0f, -90.0f, 0.0f), scale(1.0f, 1.0f, 1.0f)
{
	updateVectors();
}

Matrix4 Transform::getMatrix() const
{
	Matrix4 parentMatrix;
	if (parent != nullptr)
	{
		parentMatrix = parent->getMatrix();
	}
	return parentMatrix * Matrix4::Translation(position) * Matrix4::Rotation(rotation) * Matrix4::Scaling(scale);
}

void Transform::setRotation(const Vector3& rotation) {
	this->rotation = rotation;

	updateVectors();
}

void Transform::lookAt(const Vector3& target)
{
}
