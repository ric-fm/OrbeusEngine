#include "Vector3.h"

#include <math.h>
#include <assert.h>

#include "Math.h"
#include "Quaternion.h"

const Vector3 Vector3::right(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::up(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::forward(0.0f, 0.0f, 1.0f);

const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::one(1.0f, 1.0f, 1.0f);

Vector3::Vector3()
	: x(0.0f), y(0.0f), z(0.0f)
{
}

Vector3::Vector3(float x, float y, float z)
	: x(x), y(y), z(z)
{
}

Vector3::Vector3(float v)
	: x(v), y(v), z(v)
{
}

Vector3::Vector3(const Vector3& other)
	: x(other.x), y(other.y), z(other.z)
{
}

bool Vector3::operator==(const Vector3& other) const
{
	return x == other.x && y == other.y && z == other.z;
}

bool Vector3::operator!=(const Vector3& other) const
{
	return x != other.x || y != other.y || z != other.z;
}

Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator+(float v) const
{
	return Vector3(x + v, y + v, z + v);
}

Vector3& Vector3::operator+=(float v)
{
	x += v;
	y += v;
	z += v;
	return *this;
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator-(float v) const
{
	return Vector3(x - v, y - v, z - v);
}

Vector3& Vector3::operator-=(float v)
{
	x -= v;
	y -= v;
	z -= v;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3 Vector3::operator*(const Vector3& other) const
{
	return Vector3(x * other.x, y * other.y, z * other.z);
}

Vector3& Vector3::operator*=(const Vector3& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	return *this;
}

Vector3 Vector3::operator*(float v) const
{
	return Vector3(x * v, y * v, z * v);
}

Vector3 & Vector3::operator*=(float v)
{
	x *= v;
	y *= v;
	z *= v;
	return *this;
}

Vector3 Vector3::operator/(const Vector3& other) const
{
	return Vector3(x / other.x, y / other.y, z / other.z);
}

Vector3& Vector3::operator/=(const Vector3& other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
	return *this;
}

Vector3 Vector3::operator/(float v) const
{
	return Vector3(x / v, y / v, z / v);
}

Vector3& Vector3::operator/=(float v)
{
	x /= v;
	y /= v;
	z /= v;
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

float Vector3::operator[](unsigned int index) const
{
	assert(index < 3);
	if (index == 0)
	{
		return x;
	}
	else if (index == 1)
	{
		return y;
	}
	else
	{
		return z;
	}
}

float& Vector3::operator[](unsigned int index)
{
	assert(index < 3);
	if (index == 0)
	{
		return x;
	}
	else if (index == 1)
	{
		return y;
	}
	else
	{
		return z;
	}
}

void Vector3::set(float x, float y, float z)
{
	this->x = x; this->y = y; this->z = z;
}

bool Vector3::isZero() const
{
	return (x == 0.0f && y == 0.0f && z == 0.0f);
}

bool Vector3::isOne() const
{
	return (x == 1.0f && y == 1.0f && z == 1.0f);
}

float Vector3::dot(const Vector3& other) const
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3 Vector3::cross(const Vector3& other) const
{
	return Vector3
	(
		(y * other.z) - (z * other.y),
		(z * other.x) - (x * other.z),
		(x * other.y) - (y * other.x)
	);
}

void Vector3::directionAndLength(Vector3& direction, float& length) const
{
	length = this->length();
	if (length > 0.0f)
	{
		direction.x = x / length;
		direction.y = y / length;
		direction.z = z / length;
	}
	else
	{
		direction.x = direction.y = direction.z = 0.0f;
	}
}

float Vector3::length() const
{
	return (float)sqrt((x * x) + (y * y) + (z * z));
}

float Vector3::lengthSquared() const
{
	return (x * x) + (y * y) + (z * z);
}

void Vector3::normalize()
{
	float length = this->length();
	if (length != 0)
	{
		*this /= length;
	}
}

Vector3 Vector3::getNormalized() const
{
	float length = this->length();
	if (length == 0)
	{
		return Vector3();
	}
	return Vector3(x / length, y / length, z / length);
}

float Vector3::angle(const Vector3& target) const
{
	//return Math::radiansToDegrees(atan2(cross(target).length(), dot(target)));	
	return Math::radiansToDegrees(acos(dot(target)));
}

float Vector3::distance(const Vector3& target) const
{
	return (*this - target).length();
}

float Vector3::distanceSquared(const Vector3 & target) const
{
	return (*this - target).lengthSquared();
}

Vector3 Vector3::rotate(const Vector3& axis, float angle) const
{
	float radians = Math::degreesToRadians(angle);

	float s = sin(-radians);
	float c = cos(-radians);

	Vector3 xRot = axis * s;
	Vector3 yRot = axis * dot(axis * 1 - c);
	Vector3 zRot = *this * c;

	return cross(xRot + yRot + zRot);
}

Vector3 Vector3::rotate(const Quaternion& q) const
{
	// Based on Cocos2D
	//Vector3 uv, uuv;
	//Vector3 qvec(q.x, q.y, q.z);
	//uv = qvec.cross(*this);
	//uuv = qvec.cross(uv);
	//uv *= (2.0f * q.w);
	//uuv *= 2.0f;
	//return *this + uv + uuv;

	// Based on Three.js
	// quat * vector
	float ix = q.w * x + q.y * z - q.z * y;
	float iy = q.w * y + q.z * x - q.x * z;
	float iz = q.w * z + q.x * y - q.y * x;
	float iw = -q.x * x - q.y * y - q.z * z;

	// result * inverse quat
	return Vector3(
		ix * q.w + iw * -q.x + iy * -q.z - iz * -q.y,
		iy * q.w + iw * -q.y + iz * -q.x - ix * -q.z,
		iz * q.w + iw * -q.z + ix * -q.y - iy * -q.x
	);
}
