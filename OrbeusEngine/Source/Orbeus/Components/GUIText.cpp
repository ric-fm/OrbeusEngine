#include "GUIText.h"

#include "Orbeus/Rendering\Text\TextMeshData.h"
#include "Orbeus/Rendering\Text\Font.h"

#include "Orbeus/ResourceManagement\ResourceManager.h"

void GUIText::updateText()
{
	if (meshData != nullptr)
	{
		delete meshData;
	}
	meshData = font->loadText(text, fontSize);
}

GUIText::GUIText(const std::string& text, float fontSize, const std::string& fontName, const Vector3& color)
	: text(text), fontSize(fontSize), color(color)
{
	font = ResourceManager::getInstance().getFont(fontName);
	updateText();
}

GUIText::~GUIText()
{
	if (meshData != nullptr)
	{
		delete meshData;
	}
}

void GUIText::setText(const std::string& newText)
{
	if (text != newText)
	{
		text = newText;
		updateText();
	}
}

void GUIText::setFontSize(float newFontSize)
{
	if (fontSize != newFontSize)
	{
		fontSize = newFontSize;
		updateText();
	}
}
