#include "Vector2.h"

#include <math.h>
#include <assert.h> 

#include "Math.h"


const Vector2 Vector2::right(1.0f, 0.0f);
const Vector2 Vector2::up(0.0f, 1.0f);

const Vector2 Vector2::zero(0.0f, 0.0f);
const Vector2 Vector2::one(1.0f, 1.0f);

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

bool Vector2::operator==(const Vector2& other) const
{
	return x == other.x && y == other.y;
}

bool Vector2::operator!=(const Vector2& other) const
{
	return x != other.x || y != other.y;
}

Vector2 Vector2::operator+(const Vector2& other) const
{
	return Vector2(x + other.x, y + other.y);
}

Vector2 Vector2::operator+(float v) const
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

Vector2 Vector2::operator-(const Vector2& other) const
{
	return Vector2(x - other.x, y - other.y);
}

Vector2 Vector2::operator-(float v) const
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

Vector2 Vector2::operator*(const Vector2& other) const
{
	return Vector2(x * other.x, y * other.y);
}

Vector2& Vector2::operator*=(const Vector2& other)
{
	x *= other.x;
	y *= other.y;
	return *this;
}

Vector2 Vector2::operator*(float v) const
{
	return Vector2(x * v, y * v);
}

Vector2 & Vector2::operator*=(float v)
{
	x *= v;
	y *= v;
	return *this;
}

Vector2 Vector2::operator/(const Vector2& other) const
{
	return Vector2(x / other.x, y / other.y);
}

Vector2& Vector2::operator/=(const Vector2& other)
{
	x /= other.x;
	y /= other.y;
	return *this;
}

Vector2 Vector2::operator/(float v) const
{
	return Vector2(x / v, y / v);
}

Vector2& Vector2::operator/=(float v)
{
	x /= v;
	y /= v;
	return *this;
}

Vector2 Vector2::operator-() const
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

void Vector2::set(float x, float y)
{
	this->x = x; this->y = y;
}

bool Vector2::isZero() const
{
	return (x == 0.0f && y == 0.0f);
}

bool Vector2::isOne() const
{
	return (x == 1.0f && y == 1.0f);
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

void Vector2::normalize()
{
	float length = this->length();
	if (length != 0)
	{
		*this /= length;
	}
}

Vector2 Vector2::getNormalized() const
{
	float length = this->length();
	if (length == 0)
	{
		return Vector2();
	}
	return Vector2(x / length, y / length);
}

float Vector2::angle(const Vector2& target) const
{
	//return Math::radiansToDegrees(atan2(cross(target).length(), dot(target)));	
	return Math::radiansToDegrees(acos(dot(target)));
}

float Vector2::distance(const Vector2& target) const
{
	return (*this - target).length();
}

float Vector2::distanceSquared(const Vector2 & target) const
{
	return (*this - target).lengthSquared();
}
