#include "Rect.h"

Rect::Rect()
	: min(), max()
{
}

Rect::Rect(const Vector2& min, const Vector2& max)
	: min(min), max(max)
{
}

Rect::Rect(float minX, float minY, float maxX, float maxY)
	: min(minX, minY), max(maxY, maxY)
{
}

Rect::Rect(float v)
	: min(v), max(v)
{
}

void Rect::set(const Vector2& min, const Vector2& max)
{
	this->min.set(min.x, min.y);
	this->max.set(max.x, max.y);
}

void Rect::set(float minX, float minY, float maxX, float maxY)
{
	min.set(minX, minY);
	max.set(maxX, maxY);
}
