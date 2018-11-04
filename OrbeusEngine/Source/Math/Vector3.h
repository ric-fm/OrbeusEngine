#pragma once

#include "Utils/String.h"

struct Quaternion;

struct Vector3
{
	static const Vector3 right;
	static const Vector3 up;
	static const Vector3 forward;

	static const Vector3 zero;
	static const Vector3 one;

	float x;
	float y;
	float z;

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(float v);
	Vector3(const Vector3& other);

	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;

	Vector3 operator+(const Vector3& other) const;
	Vector3& operator+=(const Vector3& other);
	Vector3 operator+(float v) const;
	Vector3& operator+=(float v);

	Vector3 operator-(const Vector3& other) const;
	Vector3& operator-=(const Vector3& other);
	Vector3 operator-(float v) const;
	Vector3& operator-=(float v);

	Vector3 operator*(const Vector3& other) const;
	Vector3& operator*=(const Vector3& other);
	Vector3 operator*(float v) const;
	Vector3& operator*=(float v);

	Vector3 operator/(const Vector3& other) const;
	Vector3& operator/=(const Vector3& other);
	Vector3 operator/(float v) const;
	Vector3& operator/=(float v);

	Vector3 operator-() const;

	float operator[](unsigned int index) const;
	float& operator[](unsigned int index);

	void set(float x, float y, float z);

	bool isZero() const;
	bool isOne() const;

	float dot(const Vector3& other) const;
	Vector3 cross(const Vector3& other) const;

	void directionAndLength(Vector3& direction, float& length) const;
	float length() const;
	float lengthSquared() const;
	void normalize();
	Vector3 getNormalizez() const;

	float angle(const Vector3& target) const;
	float distance(const Vector3& target) const;
	float distanceSquared(const Vector3& target) const;

	Vector3 rotate(const Vector3& axis, float angle) const;

	Vector3 rotate(const Quaternion& q) const;

	// TODO: clamp, lerp, smooth

	std::string toString() const
	{
		return formatString("(%f, %f, %f)", x, y, z);
	}
};
