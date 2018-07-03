#include "Vector3.h"

#include <math.h>
#include <assert.h> 

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

bool Vector3::operator==(const Vector3& other)
{
	return x == other.x && y == other.y && z == other.z;
}

bool Vector3::operator!=(const Vector3& other)
{
	return x != other.x || y != other.y || z != other.z;
}

Vector3 Vector3::operator+(const Vector3& other)
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator+(float v)
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

Vector3 Vector3::operator-(const Vector3& other)
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator-(float v)
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

Vector3 Vector3::operator*(const Vector3& other)
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

Vector3 Vector3::operator*(float v)
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

Vector3 Vector3::operator/(const Vector3& other)
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

Vector3 Vector3::operator/(float v)
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

Vector3 Vector3::operator-()
{
	return Vector3(-x, -y, -z);
}

float Vector3::operator[](unsigned int index)
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

float Vector3::dot(const Vector3& other)
{
	return (x * other.x) + (y * other.y) + (z * other.z);
}

Vector3 Vector3::cross(const Vector3& other)
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

Vector3 Vector3::normalize() const
{
	float length = this->length();
	if (length == 0)
	{
		return Vector3();
	}
	return Vector3(x / length, y / length, z / length);
}
