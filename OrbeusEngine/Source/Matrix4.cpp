#include "Matrix4.h"

#include <math.h>

#include "Math.h"
#include "Vector3.h"
#include "Vector2.h"

Matrix4::Matrix4()
	: Matrix4
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	)
{
}

Matrix4::Matrix4(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15)
{
	buffer[0][0] = v0;  buffer[0][1] = v1;  buffer[0][2] = v2;  buffer[0][3] = v3;
	buffer[1][0] = v4;  buffer[1][1] = v5;  buffer[1][2] = v6;  buffer[1][3] = v7;
	buffer[2][0] = v8;  buffer[2][1] = v9;  buffer[2][2] = v10; buffer[2][3] = v11;
	buffer[3][0] = v12; buffer[3][1] = v13; buffer[3][2] = v14; buffer[3][3] = v15;
}

Matrix4::Matrix4(const Matrix4& other)
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			buffer[i][j] = other.buffer[i][j];
		}
	}
}

void Matrix4::operator=(const Matrix4& other)
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			buffer[i][j] = other.buffer[i][j];
		}
	}
}

Matrix4 Matrix4::operator*(const Matrix4& other) const
{
	Matrix4 result;

	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			float sum = 0.0f;
			for (unsigned int k = 0; k < 4; ++k)
			{
				// Post-Multiplication because column-major order
				sum += other.buffer[i][k] * buffer[k][j];
			}
			result.buffer[i][j] = sum;
		}
	}

	return result;
}

void Matrix4::operator*=(const Matrix4& other)
{
	*this = *this * other;
}

Matrix4 Matrix4::operator*(float value) const
{
	Matrix4 result;

	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			result.buffer[i][j] = buffer[i][j] * value;
		}
	}

	return result;
}

void Matrix4::operator*=(float value)
{
	*this = *this * value;
}

Matrix4 Matrix4::operator+(const Matrix4& other) const
{
	Matrix4 result;

	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			result.buffer[i][j] = buffer[i][j] + other.buffer[i][j];
		}
	}

	return result;
}

void Matrix4::operator+=(const Matrix4& other)
{
	*this = *this + other;
}

Matrix4 Matrix4::operator+(float value) const
{
	Matrix4 result;

	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			result.buffer[i][j] = buffer[i][j] + value;
		}
	}

	return result;
}

void Matrix4::operator+=(float value)
{
	*this = *this + value;
}

Matrix4 Matrix4::operator-(const Matrix4 & other) const
{
	Matrix4 result;

	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			result.buffer[i][j] = buffer[i][j] - other.buffer[i][j];
		}
	}

	return result;
}

void Matrix4::operator-=(const Matrix4 & other)
{
	*this = *this - other;
}

Matrix4 Matrix4::operator-(float value) const
{
	Matrix4 result;

	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			result.buffer[i][j] = buffer[i][j] - value;
		}
	}

	return result;
}

void Matrix4::operator-=(float value)
{
	*this = *this - value;
}

Matrix4 Matrix4::transpose() const
{
	return Matrix4
	(
		buffer[0][0],	buffer[0][1],	buffer[0][2],	buffer[0][3],
		buffer[1][0],	buffer[1][1],	buffer[1][2],	buffer[1][3],
		buffer[2][0],	buffer[2][1],	buffer[2][2],	buffer[2][3],
		buffer[3][0],	buffer[3][1],	buffer[3][2],	buffer[3][3]
	);
}

Matrix4 Matrix4::translate(const Vector3& vector) const
{
	return *this * Translation(vector);
}

Matrix4 Matrix4::rotate(const Vector3& axis, float angle) const
{
	return *this * Rotation(axis, angle);
}

Matrix4 Matrix4::scale(const Vector3& vector) const
{
	return *this * Scaling(vector);
}

Matrix4 Matrix4::lookAt(const Vector3& target) const
{
	// TODO: Implement this
	// http://www.songho.ca/opengl/gl_camera.html#lookat
	return Matrix4();
}

Matrix4 Matrix4::Identity()
{
	return Matrix4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4 Matrix4::Translation(const Vector3& vector)
{
	return Matrix4
	(
		1.0f,		0.0f,		0.0f,		0.0f,
		0.0f,		1.0f,		0.0f,		0.0f,
		0.0f,		0.0f,		1.0f,		0.0f,
		vector.x,	vector.y,	vector.z,	1.0f
	);
}

Matrix4 Matrix4::Translation(const Vector2& vector)
{
	return Matrix4
	(
		1.0f,		0.0f,		0.0f,		0.0f,
		0.0f,		1.0f,		0.0f,		0.0f,
		0.0f,		0.0f,		1.0f,		0.0f,
		vector.x,	vector.y,	0.0f,		1.0f
	);
}

Matrix4 Matrix4::Scaling(const Vector3& vector)
{
	return Matrix4
	(
		vector.x,	0.0f,		0.0f,		0.0f,
		0.0f,		vector.y,	0.0f,		0.0f,
		0.0f,		0.0f,		vector.z,	0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	);
}

Matrix4 Matrix4::Scaling(const Vector2& vector)
{
	return Matrix4
	(
		vector.x,	0.0f,		0.0f,		0.0f,
		0.0f,		vector.y,	0.0f,		0.0f,
		0.0f,		0.0f,		0.0f,		0.0f,
		0.0f,		0.0f,		0.0f,		1.0f
	);
}

Matrix4 Matrix4::Rotation(const Vector3& axis, float angle)
{
	float radians = Math::degreesToRadians(angle);

	float c = cos(radians);
	float s = sin(radians);
	float ic = 1.0f - c;

	return Matrix4
	(
		c + axis.x * axis.x * ic,				axis.y * axis.x * ic + axis.z * s,				axis.z * axis.x * ic - axis.y * s,		0.0f,
		axis.x * axis.y * ic - axis.z * s,		c + axis.y * axis.y * ic,						axis.z * axis.y * ic + axis.x * s,		0.0f,
		axis.x * axis.z * ic + axis.y * s,		axis.y * axis.z * ic - axis.x * s,				c + axis.z * axis.z * ic,				0.0f,
		0.0f,									0.0f,											0.0f,									1.0f
	);	
}

Matrix4 Matrix4::Rotation(const Vector3& angles)
{
	Vector3 axis;
	float angle;
	angles.directionAndLength(axis, angle);

	return Rotation(axis, angle);
}

Matrix4 Matrix4::Orthographic(float left, float right, float bottom, float top, float near, float far)
{
	return Matrix4
	(
		2.0f / (right - left),					0.0f,									0.0f,									0.0f,
		0.0f,									2.0f / (top - bottom),					0.0f,									0.0f,
		0.0f,									0.0f,									-2.0f / (far - near),					0.0f,
		-(right + left) / (right - left),		-(top + bottom) / (top - bottom),		-(far + near) / (far - near),			1.0f
	);
}

Matrix4 Matrix4::Perspective(float fov, float aspectRatio, float near, float far)
{
	float f = 1.0f / tan(Math::degreesToRadians(fov) * 0.5f);

	return Matrix4
	(
		f / aspectRatio,						0.0f,									0.0f,									0.0f,
		0.0f,									f,										0.0f,									0.0f,
		0.0f,									0.0f,									(far + near) / (near - far),			-1.0f,
		0.0f,									0.0f,									2.0f * far * near / (near - far),		0.0f
	);
}
