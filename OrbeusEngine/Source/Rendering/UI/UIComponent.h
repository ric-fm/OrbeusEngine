#pragma once

#include "Math/Matrix4.h"
#include "Math/Vector2.h"
#include "Math/Vector4.h"
#include "Math/Rect.h"

class RectTransform
{
public:
	RectTransform();
	RectTransform(Vector2 position, Vector2 size);
	~RectTransform();

	Rect& getRect() { return rect; }
	void setRect(const Rect& rect) { this->rect = rect; }

	const Vector2& getPosition() const { return position; }
	void setPosition(Vector2 position) { this->position = position; }

	const Vector2& getSize() const { return size; }
	void setSize(Vector2 size) { this->size = size; }

	const Vector2& getAnchorMin() const { return anchorMin; }
	const Vector2& getAnchorMax() const { return anchorMax; }
	void setAnchor(const Vector2& anchorMin, const Vector2& anchorMax) { this->anchorMin = anchorMin; this->anchorMax = anchorMax; }

	const Vector2& getPivot() const { return pivot; }
	void setPivot(const Vector2& pivot) { this->pivot = pivot; }

	Matrix4 getMatrix() const;

private:
	Rect rect;
	Vector2 position;
	Vector2 size;
	Vector2 pivot;
	Vector2 anchorMin;
	Vector2 anchorMax;
};


class UIComponent
{
public:
	UIComponent();
	~UIComponent();

	RectTransform& getTransform() { return transform; }
	void setTransform(const RectTransform& transform) { this->transform = transform; }

	const Vector4& getColor() const { return color; }
	void setColor(const Vector4& color) { this->color = color; }


private:
	RectTransform transform;
	Vector4 color;
};