#pragma once

#include "Math/Vector2.h"

struct Rect
{
	Vector2 min;
	Vector2 max;

	Rect();
	Rect(const Vector2& min, const Vector2& max);
	Rect(float minX, float minY, float maxX, float maxY);
	Rect(float v);

	void set(const Vector2& min, const Vector2& max);
	void set(float minX, float minY, float maxX, float maxY);
};