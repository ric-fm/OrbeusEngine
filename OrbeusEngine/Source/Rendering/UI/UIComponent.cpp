#include "UIComponent.h"

#include "Core/Engine.h"
#include "Window/Window.h"

RectTransform::RectTransform()
	: position(), size()
{
}

RectTransform::RectTransform(Vector2 position, Vector2 size)
	: rect(), position(position), size(size), pivot(), anchorMin(0.5f), anchorMax(0.5f)
{
}

RectTransform::~RectTransform()
{
}

Matrix4 RectTransform::getMatrix() const
{
	/* Anchor (Unity)
	* Si min == max en un eje, el componente se ancla en la posición relativa (a su padre) que marca el anchor.
	* Se pueden poner valores < 0 y > 1.
	* Si por ejempo, el anchor en x.
		* min == max, en el inspector te sale para poner la posición en x y el ancho.
		* min < max, en el inspector te sale para poner left y right, que marca la separación a los anchors, de forma que valores positivos hacen el elemento más pequeño
		* min > max, en el inspector sale para poner left y right, que marca la separación a los anchors, de forma que con valores de left y right a 0, el componente "está
		escalado a -1" y parece que está dado la vuelta y no se ve, y se marca como error. Para que se viera, el padre tendría que estar al revés o dar valores negativos hasta
		volver a darles la vuelta.
	*/

	Vector2 parentSize(Engine::getInstance().getWindow()->getWidth(), Engine::getInstance().getWindow()->getHeight()); // for testing

	if (true)
	{
		Rect pRect;

		pRect.min = anchorMin * parentSize;
		pRect.max = anchorMax * parentSize;

		if (anchorMin.x != anchorMax.x)
		{
			pRect.min.x += rect.min.x;
			pRect.max.x -= rect.max.x;
		}
		else
		{
			pRect.min.x -= pivot.x * size.x;
			pRect.max.x = pRect.min.x + size.x;
		}

		if (anchorMin.y != anchorMax.y)
		{
			pRect.min.y += rect.min.y;
			pRect.max.y -= rect.max.y;
		}
		else
		{
			pRect.min.y -= pivot.y * size.y;
			pRect.max.y = pRect.min.y + size.y;
		}

		return Matrix4::Translation(pRect.min) * Matrix4::Scaling(pRect.max - pRect.min);
	}
	else
	{
		Vector2 pos = position;

		Vector2 pivotOffset = pivot * size;
		pos -= pivotOffset;


		Vector2 anchorOffset = anchorMin * parentSize;
		pos += anchorOffset;

		return Matrix4::Translation(pos) * Matrix4::Scaling(size);
	}
}

UIComponent::UIComponent()
	: transform(), color(1.0f)
{
}

UIComponent::~UIComponent()
{
}
