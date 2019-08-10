#pragma once

#include "Orbeus/Utils/String.h"

struct Vector4
{

	float x;
	float y;
	float z;
	float w;

	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(float v);
	Vector4(const Vector4& other);

	bool operator==(const Vector4& other) const;
	bool operator!=(const Vector4& other) const;

	Vector4 operator+(const Vector4& other) const;
	Vector4& operator+=(const Vector4& other);
	Vector4 operator+(float v) const;
	Vector4& operator+=(float v);

	Vector4 operator-(const Vector4& other) const;
	Vector4& operator-=(const Vector4& other);
	Vector4 operator-(float v) const;
	Vector4& operator-=(float v);

	Vector4 operator*(const Vector4& other) const;
	Vector4& operator*=(const Vector4& other);
	Vector4 operator*(float v) const;
	Vector4& operator*=(float v);

	Vector4 operator/(const Vector4& other) const;
	Vector4& operator/=(const Vector4& other);
	Vector4 operator/(float v) const;
	Vector4& operator/=(float v);

	Vector4 operator-() const;

	float operator[](unsigned int index) const;
	float& operator[](unsigned int index);

	void set(float x, float y, float z, float w);

	bool isZero() const;
	bool isOne() const;

	float dot(const Vector4& other) const;

	float length() const;
	float lengthSquared() const;
	void normalize();
	Vector4 getNormalized() const;

	float angle(const Vector4& target) const;
	float distance(const Vector4& target) const;
	float distanceSquared(const Vector4& target) const;


	// TODO: clamp, lerp, smooth

	std::string toString() const
	{
		return formatString("(%f, %f, %f)", x, y, z);
	}
};
