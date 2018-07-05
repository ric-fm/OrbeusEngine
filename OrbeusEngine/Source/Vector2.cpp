#include "Vector2.h"

#include <math.h>
#include <assert.h> 

Vector2::Vector2()
	: x(0.0f), y(0.0f)
{
}

Vector2::Vector2(float x, float y)
	: x(x), y(y)
{
}

Vector2::Vector2(float v)
	: x(v), y(v)
{
}

Vector2::Vector2(const Vector2& other)
	: x(other.x), y(other.y)
{
}

bool Vector2::operator==(const Vector2& other)
{
	return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other)
{
	return x != other.x || y != other.y;
}

Vector2 Vector2::operator+(const Vector2& other)
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator+(float v)
{
	return Vector2(x + v, y + v);
}

Vector2& Vector2::operator+=(float v)
{
	x += v;
	y += v;
	return *this;
}

Vector2& Vector2::operator+=(const Vector2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vector2 Vector2::operator-(const Vector2& other)
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator-(float v)
{
	return Vector2(x - v, y - v);
}

Vector2& Vector2::operator-=(float v)
{
	x -= v;
	y -= v;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector2 Vector2::operator*(const Vector2& other)
{
	return Vector2(x * other.x, y * other.y);
}

Vector2& Vector2::operator*=(const Vector2& other)
{
	x *= other.x;
	y *= other.y;
	return *this;
}

Vector2 Vector2::operator*(float v)
{
	return Vector2(x * v, y * v);
}

Vector2 & Vector2::operator*=(float v)
{
	x *= v;
	y *= v;
	return *this;
}

Vector2 Vector2::operator/(const Vector2& other)
{
	return Vector2(x / other.x, y / other.y);
}

Vector2& Vector2::operator/=(const Vector2& other)
{
	x /= other.x;
	y /= other.y;
	return *this;
}

Vector2 Vector2::operator/(float v)
{
	return Vector2(x / v, y / v);
}

Vector2& Vector2::operator/=(float v)
{
	x /= v;
	y /= v;
	return *this;
}

Vector2 Vector2::operator-()
{
	return Vector2(-x, -y);
}

float Vector2::operator[](unsigned int index) const
{
	assert(index < 2);
	return (index == 0 ? x : y);
}

float& Vector2::operator[](unsigned int index)
{
	assert(index < 2);
	return (index == 0 ? x : y);
}

float Vector2::dot(const Vector2& other) const
{
	return (x * other.x) + (y * other.y);
}

void Vector2::directionAndLength(Vector2& direction, float& length) const
{
	length = this->length();
	if (length > 0.0f)
	{
		direction.x = x / length;
		direction.y = y / length;
	}
	else
	{
		direction.x = direction.y = 0.0f;
	}
}

float Vector2::length() const
{
	return (float)sqrt((x * x) + (y * y));
}

float Vector2::lengthSquared() const
{
	return (x * x) + (y * y);
}

Vector2 Vector2::normalize() const
{
	float length = this->length();
	if (length == 0)
	{
		return Vector2();
	}
	return Vector2(x / length, y / length);
}
