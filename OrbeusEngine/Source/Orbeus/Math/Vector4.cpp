#include "Vector4.h"

#include <math.h>
#include <assert.h>

#include "Math.h"



Vector4::Vector4()
	: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}

Vector4::Vector4(float x, float y, float z, float w)
	: x(x), y(y), z(z), w(w)
{
}

Vector4::Vector4(float v)
	: x(v), y(v), z(v), w(v)
{
}

Vector4::Vector4(const Vector4& other)
	: x(other.x), y(other.y), z(other.z), w(other.w)
{
}

bool Vector4::operator==(const Vector4& other) const
{
	return x == other.x && y == other.y && z == other.z && w == other.w;
}

bool Vector4::operator!=(const Vector4& other) const
{
	return x != other.x || y != other.y || z != other.z || w != other.w;
}

Vector4 Vector4::operator+(const Vector4& other) const
{
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}

Vector4 Vector4::operator+(float v) const
{
	return Vector4(x + v, y + v, z + v, w + v);
}

Vector4& Vector4::operator+=(float v)
{
	x += v;
	y += v;
	z += v;
	w += v;
	return *this;
}

Vector4& Vector4::operator+=(const Vector4& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

Vector4 Vector4::operator-(const Vector4& other) const
{
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}

Vector4 Vector4::operator-(float v) const
{
	return Vector4(x - v, y - v, z - v, w - v);
}

Vector4& Vector4::operator-=(float v)
{
	x -= v;
	y -= v;
	z -= v;
	w -= v;
	return *this;
}

Vector4& Vector4::operator-=(const Vector4& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

Vector4 Vector4::operator*(const Vector4& other) const
{
	return Vector4(x * other.x, y * other.y, z * other.z, w * other.w);
}

Vector4& Vector4::operator*=(const Vector4& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
	w *= other.w;
	return *this;
}

Vector4 Vector4::operator*(float v) const
{
	return Vector4(x * v, y * v, z * v, w * v);
}

Vector4 & Vector4::operator*=(float v)
{
	x *= v;
	y *= v;
	z *= v;
	w *= v;
	return *this;
}

Vector4 Vector4::operator/(const Vector4& other) const
{
	return Vector4(x / other.x, y / other.y, z / other.z, w / other.w);
}

Vector4& Vector4::operator/=(const Vector4& other)
{
	x /= other.x;
	y /= other.y;
	z /= other.z;
	w /= other.w;
	return *this;
}

Vector4 Vector4::operator/(float v) const
{
	return Vector4(x / v, y / v, z / v, w / v);
}

Vector4& Vector4::operator/=(float v)
{
	x /= v;
	y /= v;
	z /= v;
	return *this;
}

Vector4 Vector4::operator-() const
{
	return Vector4(-x, -y, -z, -w);
}

float Vector4::operator[](unsigned int index) const
{
	assert(index < 4);
	if (index == 0)
	{
		return x;
	}
	else if (index == 1)
	{
		return y;
	}
	else if (index == 2)
	{
		return z;
	}
	else
	{
		return w;
	}
}

float& Vector4::operator[](unsigned int index)
{
	assert(index < 4);
	if (index == 0)
	{
		return x;
	}
	else if (index == 1)
	{
		return y;
	}
	else if (index == 2)
	{
		return z;
	}
	else
	{
		return w;
	}
}

void Vector4::set(float x, float y, float z, float w)
{
	this->x = x; this->y = y; this->z = z; this->w = w;
}

bool Vector4::isZero() const
{
	return (x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f);
}

bool Vector4::isOne() const
{
	return (x == 1.0f && y == 1.0f && z == 1.0f && w == 1.0f);
}

float Vector4::dot(const Vector4& other) const
{
	return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

float Vector4::length() const
{
	return (float)sqrt((x * x) + (y * y) + (z * z) + (w * w));
}

float Vector4::lengthSquared() const
{
	return (x * x) + (y * y) + (z * z) + (w * w);
}

void Vector4::normalize()
{
	float length = this->length();
	if (length != 0)
	{
		*this /= length;
	}
}

Vector4 Vector4::getNormalized() const
{
	float length = this->length();
	if (length == 0)
	{
		return Vector4();
	}
	return Vector4(x / length, y / length, z / length, w / length);
}

float Vector4::angle(const Vector4& target) const
{
	return Math::radiansToDegrees(acos(dot(target)));
}

float Vector4::distance(const Vector4& target) const
{
	return (*this - target).length();
}

float Vector4::distanceSquared(const Vector4 & target) const
{
	return (*this - target).lengthSquared();
}
