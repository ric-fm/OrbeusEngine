#include "Transform.h"

#include "Orbeus/Core/World.h"
#include "Orbeus/Core/GameObject.h"
#include "Orbeus/Math/Math.h"


Transform::Transform(GameObject* owner)
	: owner(owner), position(), rotation(), scale(1.0f, 1.0f, 1.0f)
{
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

void Transform::setRelativeRotation(const Quaternion & rotation)
{
	this->rotation = rotation;
}

Vector3 Transform::getPosition() const
{
	Matrix4 matrix = this->getMatrix();

	return Vector3(matrix.buffer[3][0], matrix.buffer[3][1], matrix.buffer[3][2]);
}

Vector3 Transform::getScale() const
{
	Matrix4 matrix = this->getMatrix();

	return Vector3(
		Vector3(matrix.buffer[0][0], matrix.buffer[0][1], matrix.buffer[0][2]).length(),
		Vector3(matrix.buffer[1][0], matrix.buffer[1][1], matrix.buffer[1][2]).length(),
		Vector3(matrix.buffer[2][0], matrix.buffer[2][1], matrix.buffer[2][2]).length()
	);
}

Quaternion Transform::getRotation() const
{
	return Quaternion::RotationMatrix(getMatrix());
}

Vector3 Transform::transformVector(const Vector3 & source) const
{
	Vector3 result;

	Matrix4 matrix = this->getMatrix();

	result.x = source.x * matrix.buffer[0][0] + source.y * matrix.buffer[1][0] + source.z * matrix.buffer[2][0];
	result.y = source.x * matrix.buffer[0][1] + source.y * matrix.buffer[1][1] + source.z * matrix.buffer[2][1];
	result.z = source.x * matrix.buffer[0][2] + source.y * matrix.buffer[1][2] + source.z * matrix.buffer[2][2];

	return result;
}

Vector3 Transform::getForwardVector() const
{
	return Vector3::forward.rotate(getRotation());
}

Vector3 Transform::getUpVector() const
{
	return Vector3::up.rotate(getRotation());
}

Vector3 Transform::getRightVector() const
{
	return Vector3::right.rotate(getRotation());
}
