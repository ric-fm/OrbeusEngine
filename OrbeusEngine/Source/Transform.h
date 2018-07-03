#pragma once

#include "Vector3.h"
#include "Matrix4.h"


struct Transform
{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	Transform();
	Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale);

	Matrix4 getMatrix();
};