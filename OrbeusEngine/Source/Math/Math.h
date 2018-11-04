#pragma once

#include <math.h>
#include <algorithm>


class Math
{
public:
	static const float PI;
	static const float Epsilon;
	static const float Tolerance;

	static float degreesToRadians(float angle);
	static float radiansToDegrees(float angle);

	static float clamp(float value, float min, float max);
	static int clamp(int value, int min, int max);
};
