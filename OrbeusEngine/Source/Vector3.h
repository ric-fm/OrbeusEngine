#pragma once


struct Vector3
{
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(float v);
	Vector3(const Vector3& other);

	bool operator==(const Vector3& other);
	bool operator!=(const Vector3& other);

	Vector3 operator+(const Vector3& other);
	Vector3& operator+=(const Vector3& other);
	Vector3 operator+(float v);
	Vector3& operator+=(float v);

	Vector3 operator-(const Vector3& other);
	Vector3& operator-=(const Vector3& other);
	Vector3 operator-(float v);
	Vector3& operator-=(float v);

	Vector3 operator*(const Vector3& other);
	Vector3& operator*=(const Vector3& other);
	Vector3 operator*(float v);
	Vector3& operator*=(float v);

	Vector3 operator/(const Vector3& other);
	Vector3& operator/=(const Vector3& other);
	Vector3 operator/(float v);
	Vector3& operator/=(float v);

	Vector3 operator-();

	float operator[](unsigned int index);

	float dot(const Vector3& other);
	Vector3 cross(const Vector3& other);

	void directionAndLength(Vector3& direction, float& length) const;
	float length() const;
	float lengthSquared() const;
	Vector3 normalize() const;
};
