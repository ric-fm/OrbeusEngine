#pragma once

#include <math.h>
#include <algorithm>


#define PI 3.14159265f


class Math
{
public:
	static float degreesToRadians(float angle);
	static float radiansToDegrees(float angle);

	static float clamp(float value, float min, float max);
	static int clamp(int value, int min, int max);
};
