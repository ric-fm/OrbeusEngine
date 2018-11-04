#pragma once

#include "Utils/String.h"

struct Vector2
{
	static const Vector2 right;
	static const Vector2 up;

	static const Vector2 zero;
	static const Vector2 one;

	float x;
	float y;

	Vector2();
	Vector2(float x, float y);
	Vector2(float v);
	Vector2(const Vector2& other);

	bool operator==(const Vector2& other) const;
	bool operator!=(const Vector2& other) const;

	Vector2 operator+(const Vector2& other) const;
	Vector2& operator+=(const Vector2& other);
	Vector2 operator+(float v) const;
	Vector2& operator+=(float v);

	Vector2 operator-(const Vector2& other) const;
	Vector2& operator-=(const Vector2& other);
	Vector2 operator-(float v) const;
	Vector2& operator-=(float v);

	Vector2 operator*(const Vector2& other) const;
	Vector2& operator*=(const Vector2& other);
	Vector2 operator*(float v) const;
	Vector2& operator*=(float v);

	Vector2 operator/(const Vector2& other) const;
	Vector2& operator/=(const Vector2& other);
	Vector2 operator/(float v) const;
	Vector2& operator/=(float v);

	Vector2 operator-() const;

	float operator[](unsigned int index) const;
	float& operator[](unsigned int index);

	void set(float x, float y);

	bool isZero() const;
	bool isOne() const;

	float dot(const Vector2& other) const;

	void directionAndLength(Vector2& direction, float& length) const;
	float length() const;
	float lengthSquared() const;
	void normalize();
	Vector2 getNormalized() const;

	float angle(const Vector2& target) const;
	float distance(const Vector2& target) const;
	float distanceSquared(const Vector2& target) const;

	std::string toString() const
	{
		return formatString("(%f, %f)", x, y);
	}
};
