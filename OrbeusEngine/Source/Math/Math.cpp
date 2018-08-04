#include "Math.h"

#include "Vector2.h"
#include "Vector3.h"

const float Math::PI = 3.14159265f;


float Math::degreesToRadians(float angle)
{
	return (angle * PI) / 180;
}

float Math::radiansToDegrees(float angle)
{
	return (180 * angle) / PI;
}

float Math::clamp(float value, float minValue, float maxValue)
{
	return std::min(std::max(value, minValue), maxValue);
}

int Math::clamp(int value, int minValue, int maxValue)
{
	return std::min(std::max(value, minValue), maxValue);
}
