#pragma once

#include <iostream>
#include <string>

#include "Orbeus/Utils/String.h"

struct Vector3;
struct Vector2;
struct Quaternion;

/**
* 4x4 Matrix for handle transformation
* http://www.songho.ca/opengl/gl_matrix.html
*/
struct Matrix4
{
	static const Matrix4 identity;
	static const Matrix4 zero;


	// Elements of the matrix. Represented with column - major notation (same as OpenGL)
	float buffer[4][4];

	Matrix4();

	Matrix4(
		float v0,  float v1,  float v2,  float v3,	// row 0
		float v4,  float v5,  float v6,  float v7,	// row 1
		float v8,  float v9,  float v10, float v11, // row 2
		float v12, float v13, float v14, float v15	// row 3
	);

	Matrix4(const Matrix4& other);

	void operator=(const Matrix4& other);

	Matrix4 operator*(const Matrix4& other) const;
	void operator*=(const Matrix4& other);
	Matrix4 operator*(float value) const;
	void operator*=(float value);

	Matrix4 operator+(const Matrix4& other) const;
	void operator+=(const Matrix4& other);
	Matrix4 operator+(float value) const;
	void operator+=(float value);

	Matrix4 operator-(const Matrix4& other) const;
	void operator-=(const Matrix4& other);
	Matrix4 operator-(float value) const;
	void operator-=(float value);

	void set(
		float v0, float v1, float v2, float v3,
		float v4, float v5, float v6, float v7,
		float v8, float v9, float v10, float v11,
		float v12, float v13, float v14, float v15
	);

	Matrix4 getTransposed() const;
	void transpose();

	Matrix4 getInversed() const;
	void inverse();

	float determinant() const;

	Vector3 getTranslation() const;
	Vector3 getScale() const;
	Quaternion getRotation() const;

	void decompose(Vector3& translation, Vector3& scale, Quaternion& rotation) const;

	static Matrix4 Translation(const Vector3& vector);
	static Matrix4 Translation(const Vector2& vector);

	static Matrix4 Scaling(const Vector3& vector);
	static Matrix4 Scaling(const Vector2& vector);

	static Matrix4 Rotation(const Vector3& axis, float angle);
	static Matrix4 Rotation(const Vector3& angles);
	static Matrix4 Rotation(const Quaternion& quaternion);

	static Matrix4 Orthographic(float left, float right, float bottom, float top, float near, float far);
	static Matrix4 Perspective(float fov, float aspectRatio, float near, float far);

	static Matrix4 LookAt(const Vector3& eye, const Vector3& target, const Vector3& upDir); // TODO: Test this and move to Transform

	inline friend std::ostream & operator<<(std::ostream& out, const Matrix4& matrix) {
		for (unsigned int i = 0; i < 4; ++i)
		{
			out <<  matrix.buffer[i][0] << ", " << matrix.buffer[i][1] << ", " << matrix.buffer[i][2] << ", " << matrix.buffer[i][3] << std::endl;
		}

		return out;
	}

	std::string toString()
	{
		return formatString(
			"%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f\n%f, %f, %f, %f",
			buffer[0][0],	buffer[0][1],	buffer[0][2],	buffer[0][3],
			buffer[1][0],	buffer[1][1],	buffer[1][2],	buffer[1][3],
			buffer[2][0],	buffer[2][1],	buffer[2][2],	buffer[2][3],
			buffer[3][0],	buffer[3][1],	buffer[3][2],	buffer[3][3]
		);
	}
};
