#pragma once

#include "Orbeus/Utils/String.h"

struct Matrix4;
struct Vector3;

struct Quaternion
{
	static const Quaternion identity;
	static const Quaternion zero;

	float x;
	float y;
	float z;
	float w;

	Quaternion();

	Quaternion(float x, float y, float z, float w);

	Quaternion(const Quaternion& other);

	float length() const;
	float lengthSquared() const;

	Quaternion getNormalized() const;
	void normalize();
	Quaternion getConjugated() const;
	void conjugate();

	Quaternion operator*(const Quaternion& other) const;
	Quaternion& operator*=(const Quaternion& other);

	Quaternion operator-() const;

	Quaternion operator+(const Quaternion& other) const;
	Quaternion& operator+=(const Quaternion& other);

	Quaternion operator-(const Quaternion& other) const;
	Quaternion& operator-=(const Quaternion& other);

	float operator[](unsigned int index) const;
	float& operator[](unsigned int index);

	void set(float x, float y, float z, float w);

	bool isIdentity() const;
	bool isZero() const;

	float dot(const Quaternion& other) const;

	Matrix4 getMatrix() const;
	void getAxisAngle(Vector3& axis, float& angle) const;
	Vector3 getEulerAngles() const;


	static Quaternion RotationMatrix(const Matrix4& m);
	static Quaternion AxisAngle(const Vector3& axis, float angle);
	static Quaternion EulerAngles(const Vector3& angles);

	static Quaternion Lerp(const Quaternion& a, const Quaternion& b, float t);

	// TODO: inverse, slerp, squad, lookto

	std::string toString() const
	{
		return formatString("(%f, %f, %f, %f)", x, y, z, w);
	}
};
