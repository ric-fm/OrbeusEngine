#pragma once

#include "Core/GameComponent.h"

#include <string>

#include "Math/Vector3.h"

class TextMeshData;
class Font;

class GUIText : public GameComponent
{
private:
	std::string text;
	float fontSize;
	Vector3 color;

	TextMeshData* meshData;
	Font* font;

	void updateText();

public:
	GUIText(const std::string& text = "", float fontSize = 1.0f, const std::string& fontName = "Arial", const Vector3& color = Vector3(1.0f));
	~GUIText();

	void setText(const std::string& newText);
	std::string getText() const { return text; }

	void setFontSize(float newFontSize);
	float getFontSize() const { return fontSize; }

	void setColor(const Vector3& newColor) { color = newColor; }
	Vector3 getColor() const { return color; }

	TextMeshData* getMeshData() const { return meshData; }

	Font* getFont() { return font; }
};
