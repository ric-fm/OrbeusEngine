#include "Matrix4.h"

#include <math.h>

#include "Math.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Quaternion.h"


const Matrix4 Matrix4::identity(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

const Matrix4 Matrix4::zero(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f
);

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

void Matrix4::set(float v0, float v1, float v2, float v3, float v4, float v5, float v6, float v7, float v8, float v9, float v10, float v11, float v12, float v13, float v14, float v15)
{
	buffer[0][0] = v0;  buffer[0][1] = v1;  buffer[0][2] = v2;  buffer[0][3] = v3;
	buffer[1][0] = v4;  buffer[1][1] = v5;  buffer[1][2] = v6;  buffer[1][3] = v7;
	buffer[2][0] = v8;  buffer[2][1] = v9;  buffer[2][2] = v10; buffer[2][3] = v11;
	buffer[3][0] = v12; buffer[3][1] = v13; buffer[3][2] = v14; buffer[3][3] = v15;
}

Matrix4 Matrix4::getTransposed() const
{
	return Matrix4
	(
		buffer[0][0], buffer[0][1], buffer[0][2], buffer[0][3],
		buffer[1][0], buffer[1][1], buffer[1][2], buffer[1][3],
		buffer[2][0], buffer[2][1], buffer[2][2], buffer[2][3],
		buffer[3][0], buffer[3][1], buffer[3][2], buffer[3][3]
	);
}

void Matrix4::transpose()
{
	float copy[4][4] =
	{

		buffer[0][0], buffer[0][1], buffer[0][2], buffer[0][3],
		buffer[1][0], buffer[1][1], buffer[1][2], buffer[1][3],
		buffer[2][0], buffer[2][1], buffer[2][2], buffer[2][3],
		buffer[3][0], buffer[3][1], buffer[3][2], buffer[3][3]
	};
	memcpy(*this->buffer, copy, sizeof(float) * 16);
}

Matrix4 Matrix4::getInversed() const
{
	Matrix4 result(*this);
	result.inverse();
	return result;
}

void Matrix4::inverse()
{
	float a0 = buffer[0][0] * buffer[1][1] - buffer[0][1] * buffer[1][0];
	float a1 = buffer[0][0] * buffer[1][2] - buffer[0][2] * buffer[1][0];
	float a2 = buffer[0][0] * buffer[1][3] - buffer[0][3] * buffer[1][0];

	float a3 = buffer[0][1] * buffer[1][2] - buffer[0][2] * buffer[1][1];
	float a4 = buffer[0][1] * buffer[1][3] - buffer[0][3] * buffer[1][1];

	float a5 = buffer[0][2] * buffer[1][3] - buffer[0][3] * buffer[1][2];

	float b0 = buffer[2][0] * buffer[3][1] - buffer[2][1] * buffer[3][0];
	float b1 = buffer[2][0] * buffer[3][2] - buffer[2][2] * buffer[3][0];
	float b2 = buffer[2][0] * buffer[3][3] - buffer[2][3] * buffer[3][0];

	float b3 = buffer[2][1] * buffer[3][2] - buffer[2][2] * buffer[3][1];
	float b4 = buffer[2][1] * buffer[3][3] - buffer[2][3] * buffer[3][1];
	float b5 = buffer[2][2] * buffer[3][3] - buffer[2][3] * buffer[3][2];

	// Calculate the determinant.
	float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

	// Close to zero, can't invert.
	if (std::abs(det) > Math::Tolerance)
	{
		Matrix4 inverse;
		inverse.buffer[0][0] = buffer[1][1] * b5 - buffer[1][2] * b4 + buffer[1][3] * b3;
		inverse.buffer[0][1] = -buffer[0][1] * b5 + buffer[0][2] * b4 - buffer[0][3] * b3;
		inverse.buffer[0][2] = buffer[3][1] * a5 - buffer[3][2] * a4 + buffer[3][3] * a3;
		inverse.buffer[0][3] = -buffer[2][1] * a5 + buffer[2][2] * a4 - buffer[2][3] * a3;

		inverse.buffer[1][0] = -buffer[1][0] * b5 + buffer[1][2] * b2 - buffer[1][3] * b1;
		inverse.buffer[1][1] = buffer[0][0] * b5 - buffer[0][2] * b2 + buffer[0][3] * b1;
		inverse.buffer[1][2] = -buffer[3][0] * a5 + buffer[3][2] * a2 - buffer[3][3] * a1;
		inverse.buffer[1][3] = buffer[2][0] * a5 - buffer[2][2] * a2 + buffer[2][3] * a1;

		inverse.buffer[2][0] = buffer[1][0] * b4 - buffer[1][1] * b2 + buffer[1][3] * b0;
		inverse.buffer[2][1] = -buffer[0][0] * b4 + buffer[0][1] * b2 - buffer[0][3] * b0;
		inverse.buffer[2][2] = buffer[3][0] * a4 - buffer[3][1] * a2 + buffer[3][3] * a0;
		inverse.buffer[2][3] = -buffer[2][0] * a4 + buffer[2][1] * a2 - buffer[2][3] * a0;

		inverse.buffer[3][0] = -buffer[1][0] * b3 + buffer[1][1] * b1 - buffer[1][2] * b0;
		inverse.buffer[3][1] = buffer[0][0] * b3 - buffer[0][1] * b1 + buffer[0][2] * b0;
		inverse.buffer[3][2] = -buffer[3][0] * a3 + buffer[3][1] * a1 - buffer[3][2] * a0;
		inverse.buffer[3][3] = buffer[2][0] * a3 - buffer[2][1] * a1 + buffer[2][2] * a0;

		inverse *= 1.0f / det;

		*this = inverse * (1.0f / det);
	}
}

float Matrix4::determinant() const
{
	float a0 = buffer[0][0] * buffer[1][1] - buffer[0][1] * buffer[1][0];
	float a1 = buffer[0][0] * buffer[1][2] - buffer[0][2] * buffer[1][0];
	float a2 = buffer[0][0] * buffer[1][3] - buffer[0][3] * buffer[1][0];

	float a3 = buffer[0][1] * buffer[1][2] - buffer[0][2] * buffer[1][1];
	float a4 = buffer[0][1] * buffer[1][3] - buffer[0][3] * buffer[1][1];

	float a5 = buffer[0][2] * buffer[1][3] - buffer[0][3] * buffer[1][2];

	float b0 = buffer[2][0] * buffer[3][1] - buffer[2][1] * buffer[3][0];
	float b1 = buffer[2][0] * buffer[3][2] - buffer[2][2] * buffer[3][0];
	float b2 = buffer[2][0] * buffer[3][3] - buffer[2][3] * buffer[3][0];

	float b3 = buffer[2][1] * buffer[3][2] - buffer[2][2] * buffer[3][1];
	float b4 = buffer[2][1] * buffer[3][3] - buffer[2][3] * buffer[3][1];
	float b5 = buffer[2][2] * buffer[3][3] - buffer[2][3] * buffer[3][2];

	return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}

Matrix4 Matrix4::Translation(const Vector3& vector)
{
	return Matrix4
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		vector.x, vector.y, vector.z, 1.0f
	);
}

Matrix4 Matrix4::Translation(const Vector2& vector)
{
	return Matrix4
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		vector.x, vector.y, 0.0f, 1.0f
	);
}

Matrix4 Matrix4::Scaling(const Vector3& vector)
{
	return Matrix4
	(
		vector.x, 0.0f, 0.0f, 0.0f,
		0.0f, vector.y, 0.0f, 0.0f,
		0.0f, 0.0f, vector.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4 Matrix4::Scaling(const Vector2& vector)
{
	return Matrix4
	(
		vector.x, 0.0f, 0.0f, 0.0f,
		0.0f, vector.y, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
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
		c + axis.x * axis.x * ic, axis.y * axis.x * ic + axis.z * s, axis.z * axis.x * ic - axis.y * s, 0.0f,
		axis.x * axis.y * ic - axis.z * s, c + axis.y * axis.y * ic, axis.z * axis.y * ic + axis.x * s, 0.0f,
		axis.x * axis.z * ic + axis.y * s, axis.y * axis.z * ic - axis.x * s, c + axis.z * axis.z * ic, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4 Matrix4::Rotation(const Vector3& angles)
{
	Vector3 axis;
	float angle;
	angles.directionAndLength(axis, angle);

	return Rotation(axis, angle);
}

Matrix4 Matrix4::Rotation(const Quaternion& q)
{
	float x2 = q.x + q.x;
	float y2 = q.y + q.y;
	float z2 = q.z + q.z;

	float xx2 = q.x * x2;
	float yy2 = q.y * y2;
	float zz2 = q.z * z2;
	float xy2 = q.x * y2;
	float xz2 = q.x * z2;
	float yz2 = q.y * z2;
	float wx2 = q.w * x2;
	float wy2 = q.w * y2;
	float wz2 = q.w * z2;

	Matrix4 dst;

	dst.buffer[0][0] = 1.0f - yy2 - zz2;
	dst.buffer[0][1] = xy2 + wz2;
	dst.buffer[0][2] = xz2 - wy2;
	dst.buffer[0][3] = 0.0f;

	dst.buffer[1][0] = xy2 - wz2;
	dst.buffer[1][1] = 1.0f - xx2 - zz2;
	dst.buffer[1][2] = yz2 + wx2;
	dst.buffer[1][3] = 0.0f;

	dst.buffer[2][0] = xz2 + wy2;
	dst.buffer[2][1] = yz2 - wx2;
	dst.buffer[2][2] = 1.0f - xx2 - yy2;
	dst.buffer[2][3] = 0.0f;

	dst.buffer[3][0] = 0.0f;
	dst.buffer[3][1] = 0.0f;
	dst.buffer[3][2] = 0.0f;
	dst.buffer[3][3] = 1.0f;

	return dst;
}

Matrix4 Matrix4::Orthographic(float left, float right, float bottom, float top, float near, float far)
{
	return Matrix4
	(
		2.0f / (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
		0.0f, 0.0f, -2.0f / (far - near), 0.0f,
		-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0f
	);
}

Matrix4 Matrix4::Perspective(float fov, float aspectRatio, float near, float far)
{
	float f = 1.0f / tan(Math::degreesToRadians(fov) * 0.5f);

	return Matrix4
	(
		f / aspectRatio, 0.0f, 0.0f, 0.0f,
		0.0f, f, 0.0f, 0.0f,
		0.0f, 0.0f, (far + near) / (near - far), -1.0f,
		0.0f, 0.0f, 2.0f * far * near / (near - far), 0.0f
	);
}

Matrix4 Matrix4::LookAt(const Vector3& eye, const Vector3& target, const Vector3& upDir)
{
	Vector3 forward = (eye - target).getNormalized();
	Vector3 left = upDir.cross(forward).getNormalized();
	Vector3 up = forward.cross(left);

	float tx = -left.x * eye.x - left.y * eye.y - left.z * eye.z;
	float ty = -up.x * eye.x - up.y * eye.y - up.z * eye.z;
	float tz = -forward.x * eye.x - forward.y * eye.y - forward.z * eye.z;

	return Matrix4
	(
		left.x, up.x, forward.x, 0.0f,
		left.y, up.y, forward.y, 0.0f,
		left.z, up.z, forward.z, 0.0f,
		tx, ty, tz, 1.0f
	);
}

Vector3 Matrix4::getTranslation() const
{
	return Vector3(buffer[3][0], buffer[3][1], buffer[3][2]);
}

Vector3 Matrix4::getScale() const
{
	Vector3 xAxis(buffer[0][0], buffer[0][1], buffer[0][2]);
	Vector3 yAxis(buffer[1][0], buffer[1][1], buffer[1][2]);
	Vector3 zAxis(buffer[2][0], buffer[2][1], buffer[2][2]);

	float det = determinant();

	return Vector3(
		xAxis.length(),
		yAxis.length(),
		det >= 0 ? zAxis.length() : -zAxis.length()
	);
}

Quaternion Matrix4::getRotation() const
{
	Vector3 xAxis(buffer[0][0], buffer[0][1], buffer[0][2]);
	Vector3 yAxis(buffer[1][0], buffer[1][1], buffer[1][2]);
	Vector3 zAxis(buffer[2][0], buffer[2][1], buffer[2][2]);

	float det = determinant();

	Vector3 scale = Vector3(
		xAxis.length(),
		yAxis.length(),
		det >= 0 ? zAxis.length() : -zAxis.length()
	);

	xAxis *= 1.0f / scale.x;
	yAxis *= 1.0f / scale.y;
	zAxis *= 1.0f / scale.z;

	float trace = xAxis.x + yAxis.y + zAxis.z + 1.0f;

	if (trace > Math::Epsilon)
	{
		float s = 0.5f / sqrt(trace);
		return Quaternion(
			(yAxis.z - zAxis.y) * s,
			(zAxis.x - xAxis.z) * s,
			(xAxis.y - yAxis.x) * s,
			0.25f / s
		);
	}
	else
	{
		if (xAxis.x > yAxis.y && xAxis.x > zAxis.z)
		{
			float s = 0.5f / sqrt(1.0f + xAxis.x - yAxis.y - zAxis.z);
			return Quaternion(
				0.25f / s,
				(yAxis.x + xAxis.y) * s,
				(zAxis.x + xAxis.z) * s,
				(yAxis.z - zAxis.y) * s
			);
		}
		else if (yAxis.y > zAxis.z)
		{
			float s = 0.5f / sqrt(1.0f + yAxis.y - xAxis.x - zAxis.z);
			return Quaternion(
				(yAxis.x + xAxis.y) * s,
				0.25f / s,
				(zAxis.y + yAxis.z) * s,
				(zAxis.x - xAxis.z) * s
			);
		}
		else
		{
			float s = 0.5f / sqrt(1.0f + zAxis.z - xAxis.x - yAxis.y);
			return Quaternion(
				(zAxis.x + xAxis.z) * s,
				(zAxis.y + yAxis.z) * s,
				0.25f / s,
				(xAxis.y - yAxis.x) * s
			);
		}
	}
}

void Matrix4::decompose(Vector3& translation, Vector3& scale, Quaternion& rotation) const
{
	// Translation
	translation = Vector3(buffer[3][0], buffer[3][1], buffer[3][2]);

	// Scale and Rotation
	Vector3 xAxis(buffer[0][0], buffer[0][1], buffer[0][2]);
	Vector3 yAxis(buffer[1][0], buffer[1][1], buffer[1][2]);
	Vector3 zAxis(buffer[2][0], buffer[2][1], buffer[2][2]);

	float det = determinant();


	// Scale
	scale = Vector3(
		xAxis.length(),
		yAxis.length(),
		det >= 0 ? zAxis.length() : -zAxis.length()
	);

	// Rotation
	xAxis *= 1.0f / scale.x;
	yAxis *= 1.0f / scale.y;
	zAxis *= 1.0f / scale.z;

	float trace = xAxis.x + yAxis.y + zAxis.z + 1.0f;

	if (trace > Math::Epsilon)
	{
		float s = 0.5f / sqrt(trace);
		rotation = Quaternion(
			(yAxis.z - zAxis.y) * s,
			(zAxis.x - xAxis.z) * s,
			(xAxis.y - yAxis.x) * s,
			0.25f / s
		);
	}
	else
	{
		if (xAxis.x > yAxis.y && xAxis.x > zAxis.z)
		{
			float s = 0.5f / sqrt(1.0f + xAxis.x - yAxis.y - zAxis.z);
			rotation = Quaternion(
				0.25f / s,
				(yAxis.x + xAxis.y) * s,
				(zAxis.x + xAxis.z) * s,
				(yAxis.z - zAxis.y) * s
			);
		}
		else if (yAxis.y > zAxis.z)
		{
			float s = 0.5f / sqrt(1.0f + yAxis.y - xAxis.x - zAxis.z);
			rotation = Quaternion(
				(yAxis.x + xAxis.y) * s,
				0.25f / s,
				(zAxis.y + yAxis.z) * s,
				(zAxis.x - xAxis.z) * s
			);
		}
		else
		{
			float s = 0.5f / sqrt(1.0f + zAxis.z - xAxis.x - yAxis.y);
			rotation = Quaternion(
				(zAxis.x + xAxis.z) * s,
				(zAxis.y + yAxis.z) * s,
				0.25f / s,
				(xAxis.y - yAxis.x) * s
			);
		}
	}
}
