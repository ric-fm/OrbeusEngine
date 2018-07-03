#include "Transform.h"

Transform::Transform()
	: position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f)
{
}

Transform::Transform(const Vector3& position, const Vector3& rotation, const Vector3& scale)
	: position(position), rotation(rotation), scale(scale)
{
}

Matrix4 Transform::getMatrix()
{
	return Matrix4::Translation(position) * Matrix4::Rotation(rotation) * Matrix4::Scaling(scale);
}
