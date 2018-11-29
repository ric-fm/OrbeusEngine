#include "Quaternion.h"

#include <math.h>
#include <assert.h> 

#include "Math.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"


const Quaternion Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);
const Quaternion Quaternion::zero(0.0f, 0.0f, 0.0f, 0.0f);

Quaternion::Quaternion()
	: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
{
}

Quaternion::Quaternion(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w)
{
}

Quaternion::Quaternion(const Quaternion& other)
	: x(other.x), y(other.y), z(other.z), w(other.w)
{
}

float Quaternion::length() const
{
	return (sqrtf(x * x + y * y + z * z + w * w));
}

float Quaternion::lengthSquared() const
{
	return x * x + y * y + z * z + w * w;
}

Quaternion Quaternion::getNormalized() const
{
	float length = this->length();
	if (length == 0.0f)
	{
		return *this;
	}
	return Quaternion(x / length, y / length, z / length, w / length);
}

void Quaternion::normalize()
{
	float length = this->length();
	if (length != 0.0f)
	{
		this->x /= length; this->y /= length; this->z /= length; this->w /= length;
	}
}

Quaternion Quaternion::getConjugated() const
{
	return Quaternion(-x, -y, -z, w);
}

void Quaternion::conjugate()
{
	x *= -1.0f; y *= -1.0f; z *= -1.0f;
}

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	return Quaternion
	(
		x * other.w + w * other.x + y * other.z - z * other.y,
		y * other.w + w * other.y + z * other.x - x * other.z,
		z * other.w + w * other.z + x * other.y - y * other.x,
		w * other.w - x * other.x - y * other.y - z * other.z
	);
}

Quaternion& Quaternion::operator*=(const Quaternion& other)
{
	return *this = *this * other;
}

Quaternion Quaternion::operator-() const
{
	return Quaternion(-x, -y, -z, -w);
}

Quaternion Quaternion::operator+(const Quaternion& other) const
{
	return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
}

Quaternion & Quaternion::operator+=(const Quaternion& other)
{
	return *this = *this + other;
}

Quaternion Quaternion::operator-(const Quaternion& other) const
{
	return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
}

Quaternion & Quaternion::operator-=(const Quaternion& other)
{
	return *this = *this - other;
}

void Quaternion::set(float x, float y, float z, float w)
{
	this->x = x; this->y = y; this->z = z; this->w = w;
}

bool Quaternion::isIdentity() const
{
	return (x == 0.0f && y == 0.0f && z == 0.0f && w == 1.0f);
}

bool Quaternion::isZero() const
{
	return (x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f);
}

float Quaternion::dot(const Quaternion& other) const
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

Matrix4 Quaternion::getMatrix() const
{
	return Matrix4::Rotation(*this);
}

void Quaternion::getAxisAngle(Vector3& axis, float& angle) const
{
	Quaternion quatNormal = getNormalized();

	axis = Vector3(quatNormal.x, quatNormal.y, quatNormal.z).getNormalized();

	angle = Math::radiansToDegrees(2.0f * acos(quatNormal.w));
}

Vector3 Quaternion::getEulerAngles() const
{
	// Based on Three.js
	// YXZ order
	Matrix4 m = Matrix4::Rotation(*this);

	float pitch = asin(-Math::clamp(m.buffer[2][1], -1.0f, 1.0f));
	float yaw, roll;

	if (abs(m.buffer[2][1]) < 0.99999) {
		yaw = atan2(m.buffer[2][0], m.buffer[2][2]);
		roll = atan2(m.buffer[0][1], m.buffer[1][1]);
		if (abs(roll) < Math::Epsilon)
		{
			roll = 0.0f;
		}
	}
	else {

		yaw = atan2(-m.buffer[0][2], m.buffer[0][0]);
		roll = 0.0f;
	}

	return Vector3(
		Math::radiansToDegrees(pitch),
		Math::radiansToDegrees(yaw),
		Math::radiansToDegrees(roll)
	);
}

Quaternion Quaternion::RotationMatrix(const Matrix4& m)
{
	return m.getRotation();
}

Quaternion Quaternion::AxisAngle(const Vector3& axis, float angle)
{
	float halfAngle = Math::degreesToRadians(angle) * 0.5f;
	Vector3 v = axis.getNormalized() * sinf(halfAngle);

	return Quaternion(
		v.x,
		v.y,
		v.z,
		cosf(halfAngle)
	);
}

Quaternion Quaternion::EulerAngles(const Vector3& angles)
{
	// Based on Three.js
	// YXZ order

	float halfX = Math::degreesToRadians(angles.x * 0.5f);
	float halfY = Math::degreesToRadians(angles.y * 0.5f);
	float halfZ = Math::degreesToRadians(angles.z * 0.5f);

	float cX = cos(halfX);
	float cY = cos(halfY);
	float cZ = cos(halfZ);

	float sX = sin(halfX);
	float sY = sin(halfY);
	float sZ = sin(halfZ);

	return Quaternion(
		sX * cY * cZ + cX * sY * sZ,
		cX * sY * cZ - sX * cY * sZ,
		cX * cY * sZ - sX * sY * cZ,
		cX * cY * cZ + sX * sY * sZ
	);
}
